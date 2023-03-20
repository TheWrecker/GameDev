
#include <cmath>
#include <DirectXMath.h>

#include "../entities/attributes/move_physics.h"
#include "../entities/attributes/collision_physics.h"
#include "../core/sys_ticker.h"
#include "game_time.h"
#include "../entities/entity_transformable.h"
#include "../entities/block_solid.h"
#include "../entities/segment.h"
#include "../entities/world.h"
#include "../visuals/scene.h"
#include "../visuals/presenter.h"
#include "../core/supervisor.h"

#include "physics_engine.h"

PhysicsEngine::PhysicsEngine(Supervisor* supervisor)
	:supervisor(supervisor), game_time(nullptr), scene(nullptr), world(nullptr), ticker(nullptr)
{
}

PhysicsEngine::~PhysicsEngine()
{
	auto _game_time = supervisor->QueryService<GameTime*>("game_time");
	if (_game_time)
		_game_time->RemoveFunction(update_task, true);

	//we are not the owner of physics components, dont delete them here
}

void PhysicsEngine::Start()
{
	ticker = supervisor->QueryService<SystemTicker*>("ticker");
	game_time = supervisor->QueryService<GameTime*>("game_time");
	scene = supervisor->QueryService<Presenter*>("presenter")->GetActiveScene();
	world = scene->GetWorld();
	//100 global physics engine ticks per second
	update_task = game_time->RegisterFunction(std::bind(&PhysicsEngine::UpdateAllSystems, this), 10, true);
}

void PhysicsEngine::Update()
{
	//service specific updates, nothing for now
	// physics components get updated each 10ms via UpdateAllSystems(), registered at GameTime
}

void PhysicsEngine::RegisterMovementComponent(MovePhysics* target)
{
	if (!target)
		return;
	movements.push_back(target);
}

void PhysicsEngine::RemoveMovementComponent(MovePhysics* target)
{
	if (!target)
		return;
	movements.remove(target);
}

void PhysicsEngine::RegisterCollisionComponent(CollisionPhysics* target)
{
	if (!target)
		return;
	collisions.push_back(target);
}

void PhysicsEngine::RemoveCollisionComponent(CollisionPhysics* target)
{
	if (!target)
		return;
	collisions.remove(target);
}

void PhysicsEngine::UpdateAllSystems()
{
	//TODO: check if global physics are enabled?
	//TODO: per-system checks?
	//TODO: per-component checks?
	//TODO: gamespeed affects movement

	//predefinitions
	//TODO: check if ticker is valid?
	auto _elapsed_time = 0.001f;//ticker->GetLastTickDuration();
	DirectX::XMVECTOR _up = { 0.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR _xz_direction, _right_vector, _front_move, _side_move, _coll;
	DirectX::XMFLOAT3 _rotation, _temp_pos;

	SolidBlock *_block1, *_block2, *_block3;
	_block1 = _block2 = _block3 = nullptr;
	TransformableEntity* _entity = nullptr;
	CollisionPhysics* _collision = nullptr;
	
	//collisions
	for (auto _component : collisions)
	{
		_component->UpdateCollisionParams();
	}

	//movements
	for (auto _component : movements)
	{
		_component->UpdateMovementParams();

		_entity = dynamic_cast<TransformableEntity*>(_component);
		_collision = dynamic_cast<CollisionPhysics*>(_entity);

		if (_entity)
		{
			//TODO: improve?

			//get position and orientation
			using namespace DirectX;
			_rotation = _entity->Rotation();
			_rotation.y = 0.0f;
			_temp_pos = _entity->Position();

			_block1 = world->GetBlock(_temp_pos.x, _temp_pos.y - (_collision->half_height + 0.1f) , _temp_pos.z);
			//is there a block directly below us?
			if (_block1)
			{
				//there is, so stop falling
				_component->falling = false;
				_component->vertical_speed = fmax(0.0f, _component->vertical_speed);
				_temp_pos.y = _block1->Position().y + SOLID_BLOCK_SIZE + _collision->half_height;
			}
			else
			{
				//there isn't, so start falling
				_component->falling = true;
			}

			//are we going upwards?
			if (_component->vertical_speed > 0.0f)
			{
				//we are going upwards, check above us
				_block1 = world->GetBlock(_temp_pos.x, _temp_pos.y + _collision->half_height, _temp_pos.z);
				//is there a block directly above us?
 				if (_block1)
				{
					//there is, so stop going upwards
					_component->vertical_speed = 0.0f;
					_temp_pos.y = _block1->Position().y - _collision->half_height;
				}
				else
				{
					//there is nothing above us for now, we can ascend freely
 					_temp_pos.y += _component->vertical_speed * _elapsed_time;
				}
			}
			//we are going downwards, check below us
			else
			{
				//there is nothing below us for now, we can fall freely aaaaaaaaaaaaaaaa
				_temp_pos.y += _component->vertical_speed * _elapsed_time;
			}

			//calculate frontal movement 2D XZ vector
			_xz_direction = XMLoadFloat3(&_rotation);
			_xz_direction = XMVector3Normalize(_xz_direction);

			_front_move = _xz_direction * _elapsed_time * _component->front_speed;
			_coll = _xz_direction * (_collision->half_width + 0.1f) * copysignf(1.0f, _component->front_speed);

			//check if we collide with blocks
			_block1 = world->GetBlock(_temp_pos.x + XMVectorGetX(_coll), _temp_pos.y, _temp_pos.z + XMVectorGetZ(_coll));
			_block2 = world->GetBlock(_temp_pos.x + XMVectorGetX(_coll), _temp_pos.y, _temp_pos.z);
			_block3 = world->GetBlock(_temp_pos.x, _temp_pos.y, _temp_pos.z + XMVectorGetZ(_coll));
			if (_block1 || _block2 || _block3)
			{
				_component->front_speed = 0.0f;
			}
			else
			{
				//check if we collide with a block
				_block1 = world->GetBlock(_temp_pos.x + XMVectorGetX(_front_move) + XMVectorGetX(_coll),
					_temp_pos.y,
					_temp_pos.z + XMVectorGetZ(_front_move) + +XMVectorGetZ(_coll));
				if (_block1)
				{
					//we do, so stop frontal movement and stand aside the block
					_component->front_speed = 0.0f;
					//auto _diffX = _block->Position().x - _temp_pos.x;
					//auto _diffY = _block->Position().y - _temp_pos.y;
					//_temp_pos.x -= _collision->half_width - _diffX - (floorf(_diffX / (_diffX * 2)) * SOLID_BLOCK_SIZE);
					//_temp_pos.z -= _collision->half_width - _diffX - (floorf(_diffY / (_diffY * 2)) * SOLID_BLOCK_SIZE);
				}
				else
				{
					//we don't so move freely (we do the check next tick)
					_temp_pos.x += XMVectorGetX(_front_move);
					_temp_pos.z += XMVectorGetZ(_front_move);
				}

			}

			//calculate side movement 2D XZ vector
			_right_vector = XMVector3Cross(_xz_direction, _up);
			_right_vector = XMVector3Normalize(_right_vector);

			_side_move = _right_vector * _elapsed_time * _component->side_speed;
			_coll = _right_vector * (_collision->half_width + 0.1f) * copysignf(1.0f, _component->side_speed);

			//check if we collide with a block
			_block1 = world->GetBlock(_temp_pos.x + XMVectorGetX(_coll), _temp_pos.y, _temp_pos.z + XMVectorGetZ(_coll));
			_block2 = world->GetBlock(_temp_pos.x + XMVectorGetX(_coll), _temp_pos.y, _temp_pos.z);
			_block3 = world->GetBlock(_temp_pos.x, _temp_pos.y, _temp_pos.z + XMVectorGetZ(_coll));
			if (_block1 || _block2 || _block3)
			{
				_component->side_speed = 0.0f;
			}
			else
			{
				//check if we collide with a block
				_block1 = world->GetBlock(_temp_pos.x + XMVectorGetX(_front_move) + XMVectorGetX(_coll),
					_temp_pos.y,
					_temp_pos.z + XMVectorGetZ(_front_move) + +XMVectorGetZ(_coll));
				if (_block1)
				{
					//we do, so stop frontal movement and stand aside the block
					_component->side_speed = 0.0f;
					//auto _diffX = _block->Position().x - _temp_pos.x;
					//auto _diffY = _block->Position().y - _temp_pos.y;
					//_temp_pos.x -= _collision->half_width - _diffX - (floorf(_diffX / (_diffX * 2)) * SOLID_BLOCK_SIZE);
					//_temp_pos.z -= _collision->half_width - _diffX - (floorf(_diffY / (_diffY * 2)) * SOLID_BLOCK_SIZE);
				}
				else
				{
					//we don't so move freely (we do the check next tick)
					_temp_pos.x += XMVectorGetX(_side_move);
					_temp_pos.z += XMVectorGetZ(_side_move);
				}

			}

			_block1 = world->GetBlock(_temp_pos.x, _temp_pos.y, _temp_pos.z);
			//are we inside a block?
			if (_block1)
			{
				//well we are but we shouldn't be
				//for now only step back once, later maybe loop untill we are 100% outside blocks
				_temp_pos.x -= _rotation.x;
				_temp_pos.z -= _rotation.z;
				_temp_pos.y += 1.0f;
			}

			_entity->SetPosition(_temp_pos);
		}
	}
}
