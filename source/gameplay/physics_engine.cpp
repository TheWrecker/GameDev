
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
	update_task = game_time->RegisterFunction(std::bind(&PhysicsEngine::UpdateAllSystems, this), 20, true);
}

void PhysicsEngine::Update()
{
	//service specific updates, nothing for now
	// physics components get updated each 20ms via UpdateAllSystems(), registered at GameTime
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


	//predefinitions
	//TODO: check if ticker is valid?
	auto _elapsed_time = ticker->GetLastTickDuration();

	DirectX::XMVECTOR _up = { 0.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR _xz_direction, _right_vector, _movement, _front_move, _side_move, _vertical_move, _position_vector;
	DirectX::XMFLOAT3 _rotation, _front_check, _side_check, _vertical_check, _temp_pos;

	SolidBlock* _block = nullptr;
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
			using namespace DirectX;
			_rotation = _entity->Rotation();
			_rotation.y = 0.0f;
			_temp_pos = _entity->Position();
			_position_vector = _entity->Position_Vector();
			_xz_direction = XMLoadFloat3(&_rotation);
			_right_vector = XMVector3Cross(_xz_direction, _up);
			XMVector3Normalize(_right_vector);

			_movement = { _component->front_speed, _component->side_speed, _component->vertical_speed, 0.0f };
			_movement *= _elapsed_time;

			_front_move = _xz_direction * XMVectorGetX(_movement);
			_side_move = _right_vector * XMVectorGetY(_movement);
			_vertical_move = _up * XMVectorGetZ(_movement);

			XMStoreFloat3(&_front_check, _front_move);
			_front_check.x < 0.0f ? _temp_pos.x -= _collision->half_width : _temp_pos.x += _collision->half_width;
			//_front_check.y < 0.0f ? _front_check.y -= _collision->half_height : _front_check.y += _collision->half_height;
			_front_check.z < 0.0f ? _temp_pos.z -= _collision->half_width : _temp_pos.z += _collision->half_width;

			_block = world->GetBlock(_temp_pos.x, _temp_pos.y, _temp_pos.z);
			if (_block)
			{
				if (!_block->collision_box.Intersects(_collision->collision_box))
					_position_vector += _front_move;
				else
					int i = 0;
			}
			else
			{
				_position_vector += _front_move;
			}

			XMStoreFloat3(&_side_check, _side_move);
			_side_check.x < 0.0f ? _temp_pos.x -= _collision->half_width : _temp_pos.x += _collision->half_width;
			//_side_check.y < 0.0f ? _side_check.y -= _collision->half_height : _side_check.y += _collision->half_height;
			_side_check.z < 0.0f ? _temp_pos.z -= _collision->half_width : _temp_pos.z += _collision->half_width;

			_block = world->GetBlock(_temp_pos.x, _temp_pos.y, _temp_pos.z);
			if (_block)
			{
				if (!_block->collision_box.Intersects(_collision->collision_box))
					_position_vector += _side_move;
				else
					int i = 0;
			}
			else
			{
				_position_vector += _side_move;
			}

			_block = world->GetBlock(_temp_pos.x, _temp_pos.y - _collision->half_height, _temp_pos.z);
			if (_block)
			{
				_component->falling = false;
			}
			else
			{
				_component->falling = true;
				XMStoreFloat3(&_vertical_check, _vertical_move);
				//_vertical_check.x < 0.0f ? _vertical_check.x -= _collision->half_width : _vertical_check.x += _collision->half_width;
				_vertical_check.y < 0.0f ? _temp_pos.y -= _collision->half_height : _temp_pos.y += _collision->half_height;
				//_vertical_check.z < 0.0f ? _vertical_check.z -= _collision->half_width : _vertical_check.z += _collision->half_width;

				_block = world->GetBlock(_temp_pos.x, _temp_pos.y, _temp_pos.z);
				if (_block)
				{
					if (!_block->collision_box.Intersects(_collision->collision_box))
						_position_vector += _vertical_move;
					else
						int i = 0;
				}
				else
				{
					_position_vector += _vertical_move;
				}
			}
			


			/*XMFLOAT3 _temp = _entity->Position();
			XMVECTOR _movement = { _component->front_speed, _component->side_speed, _component->vertical_speed, 0.0f };
			_movement *= _elapsed_time;
			XMVECTOR _up = { 0.0f, 1.0f, 0.0f, 0.0f };
			XMVECTOR _position_vector = _entity->Position_Vector();
			XMFLOAT3 _rotation = _entity->Rotation();
			XMVECTOR _direction = XMLoadFloat3(&_rotation);
			XMVECTOR _right_vector = XMVector3Cross(_direction, _up);
			XMVECTOR _strafe = _right_vector * XMVectorGetY(_movement);
			XMVECTOR _strafe_check = _right_vector * _collision->half_width * XMVectorGetY(_movement);
			_position_vector += _strafe_check;
			XMStoreFloat3(&_temp, _position_vector);
			SolidBlock* _block = world->GetBlock(_temp.x, _temp.y, _temp.z);
			if (_block)
				if (_block->collision_box.Intersects(_collision->collision_box))
				{
					_position_vector -= _strafe_check;
				}
				else
				{
					_position_vector -= _strafe_check;
					_position_vector += _strafe;
				}
			XMVECTOR _forward = _direction * XMVectorGetX(_movement);
			XMVECTOR _forward_check = _direction * _collision->half_width * XMVectorGetX(_movement);
			_position_vector += _forward_check;
			XMStoreFloat3(&_temp, _position_vector);
			_block = world->GetBlock(_temp.x, _temp.y, _temp.z);
			if (_block)
				if (_block->collision_box.Intersects(_collision->collision_box))
				{
					_position_vector -= _forward_check;
				}
				else
				{
					_position_vector -= _forward_check;
					_position_vector += _forward;
				}
			XMVECTOR _vertical = _up * XMVectorGetZ(_movement);
			XMVECTOR _vertical_check = _up * _collision->half_height * XMVectorGetZ(_movement);
			_position_vector += _vertical_check;
			XMStoreFloat3(&_temp, _position_vector);
			_block = world->GetBlock(_temp.x, _temp.y, _temp.z);
			if (_block)
				if (_block->collision_box.Intersects(_collision->collision_box))
				{
					_position_vector -= _vertical_check;
				}
				else
				{
					_position_vector -= _vertical_check;
					_position_vector += _vertical;
				}*/
			XMFLOAT3 _final_pos = {};
			XMStoreFloat3(&_final_pos, _position_vector);
			_entity->SetPosition(_final_pos);
		}
	}
}
