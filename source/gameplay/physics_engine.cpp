
#include <cmath>
#include <DirectXMath.h>

#include "physics/move_physics.h"
#include "physics/collision_physics.h"
#include "../core/sys_ticker.h"
#include "../entities/entity_transformable.h"
#include "../entities/block_solid.h"
#include "../entities/player.h"
#include "../scene/compartments/segment.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "../core/supervisor.h"

#include "physics_engine.h"

PhysicsEngine::PhysicsEngine()
	:executor(nullptr), scene(nullptr), world(nullptr), ticker(nullptr), enabled(false)
{
}

PhysicsEngine::~PhysicsEngine()
{
	if (Supervisor::GetLastInstance()->IsExecutorAvailable())
		executor->RemovePeriodicTask(update_task, true);

	//we are not the owner of physics components, dont delete them here
}

void PhysicsEngine::Start()
{
	//TODO: change the physics system to time consumption approach
	player = scene->GetPlayer();
	//100 global physics engine ticks per second
	update_task = executor->RegisterPeriodicTask(std::bind(&PhysicsEngine::UpdateAllSystems, this), 10, true);
	enabled = true;
}

bool PhysicsEngine::Initialize()
{
	ticker = Supervisor::QueryService<SystemTicker*>("ticker");
	executor = Supervisor::QueryService<Executor*>("executor");
	scene = Supervisor::QueryService<Scene*>("scene");
	world = scene->GetWorld();
	return (ticker && executor && scene && world);
}

void PhysicsEngine::Update()
{
	//service specific updates, nothing for now
	//physics components get updated each 10ms by UpdateAllSystems(), registered in the executor via Start()
	//UpdateAllSystems();
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
	//TODO: multi-threading?

	if (!enabled)
	{
		//free flying mode
		ProcessPlayerNoPhysics();
		return;
	}
	
	//TODO: per-system checks?
	//TODO: per-component checks?
	//TODO: make gamespeed affect movement speed

	//predefinitions
	//TODO: check if ticker is valid?
	auto _elapsed_time = ticker->GetLastTickDuration();
	DirectX::XMVECTOR _up = { 0.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR _xz_direction, _right_vector, _front_move, _side_move, _coll;
	DirectX::XMFLOAT3 _rotation, _temp_pos;

	BlockType _block1, _block2, _block3;
	_block1 = _block2 = _block3 = BlockType::EMPTY;
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
			if (_block1 != BlockType::EMPTY)
			{
				//there is, so stop falling
				_component->falling = false;
				_component->vertical_speed = fmax(0.0f, _component->vertical_speed);
				//_temp_pos.y = floorf(_temp_pos.y) + SOLID_BLOCK_SIZE + _collision->half_height;
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
 				if (_block1 != BlockType::EMPTY)
				{
					//there is, so stop going upwards
					_component->vertical_speed = 0.0f;
					_temp_pos.y = floorf(_temp_pos.y) - _collision->half_height;
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
			if ((_block1 != BlockType::EMPTY) || (_block2 != BlockType::EMPTY) || (_block3 != BlockType::EMPTY))
			{
				_component->front_speed = 0.0f;
			}
			else
			{
				//check if we will collide with a block
				_block1 = world->GetBlock(_temp_pos.x + XMVectorGetX(_front_move) + XMVectorGetX(_coll),
					_temp_pos.y,
					_temp_pos.z + XMVectorGetZ(_front_move) + +XMVectorGetZ(_coll));
				if (_block1 != BlockType::EMPTY)
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
			if ((_block1 != BlockType::EMPTY) || (_block2 != BlockType::EMPTY) || (_block3 != BlockType::EMPTY))
			{
				_component->side_speed = 0.0f;
			}
			else
			{
				//check if we will collide with a block
				_block1 = world->GetBlock(_temp_pos.x + XMVectorGetX(_front_move) + XMVectorGetX(_coll),
					_temp_pos.y,
					_temp_pos.z + XMVectorGetZ(_front_move) + +XMVectorGetZ(_coll));
				if (_block1 != BlockType::EMPTY)
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

			//anti-stuck temporary measurement
			_block1 = world->GetBlock(_temp_pos.x, _temp_pos.y, _temp_pos.z);
			//are we inside a block?
			//if (_block1 != BlockType::EMPTY)
			//{
			//	//well we are but we shouldn't be
			//	//for now only step back once, later maybe loop untill we are 100% outside blocks
			//	_temp_pos.x -= _rotation.x;
			//	_temp_pos.z -= _rotation.z;
			//	_temp_pos.y += 1.0f;
			//}

			_entity->SetPosition(_temp_pos);
		}
	}
}

void PhysicsEngine::ProcessPlayerNoPhysics()
{
	using namespace DirectX;
	auto _elapsed_time = ticker->GetLastTickDuration();
	XMVECTOR _up = { 0.0f, 1.0f, 0.0f, 0.0f };
	auto _front_dir = player->Rotation_Vector();
	auto _right_dir = XMVector3Cross(_front_dir, _up);
	_right_dir = XMVector3Normalize(_right_dir);
	auto _front_move = _front_dir * _elapsed_time * player->front * 20.0f;
	auto _side_move = _right_dir * _elapsed_time * player->side * 20.0f;
	auto _position = player->Position_Vector() + _front_move + _side_move;
	XMFLOAT3 _final_pos = {};
	XMStoreFloat3(&_final_pos, _position);
	player->SetPosition(_final_pos);
}
