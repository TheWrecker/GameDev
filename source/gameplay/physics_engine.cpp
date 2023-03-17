
#include <DirectXMath.h>

#include "../entities/attributes/move_physics.h"
#include "../core/sys_ticker.h"
#include "game_time.h"
#include "../entities/entity_transformable.h"
#include "../core/supervisor.h"

#include "physics_engine.h"

PhysicsEngine::PhysicsEngine(Supervisor* supervisor)
	:supervisor(supervisor)
{
	ticker = supervisor->QueryService<SystemTicker*>("ticker");
	auto _game_time = supervisor->QueryService<GameTime*>("game_time");
	update_task = _game_time->RegisterFunction(std::bind(&PhysicsEngine::UpdateAllSystems, this), 20, true);
}

PhysicsEngine::~PhysicsEngine()
{
	auto _game_time = supervisor->QueryService<GameTime*>("game_time");
	if (_game_time)
		_game_time->RemoveFunction(update_task, true);

	//we are not the owner of physics components, dont delete them here
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

void PhysicsEngine::UpdateAllSystems()
{
	//TODO: check if global physics are enabled?
	//TODO: per-system checks?
	//TODO: per-component checks?

	//TODO: check if ticker is valid?
	auto _elapsed_time = ticker->GetLastTickDuration();
	
	//movements
	for (auto _component : movements)
	{
		_component->UpdateMovementParams();

		auto _entity = dynamic_cast<TransformableEntity*>(_component);
		if (_entity)
		{
			//TODO: improve?
			using namespace DirectX;
			XMVECTOR _movement = { _component->front_speed, _component->side_speed, _component->vertical_speed, 0.0f };
			_movement *= _elapsed_time;
			XMVECTOR _up = { 0.0f, 1.0f, 0.0f, 0.0f };
			XMVECTOR _position_vector = _entity->Position_Vector();
			XMFLOAT3 _rotation = _entity->Rotation();
			XMVECTOR _direction = XMLoadFloat3(&_rotation);
			XMVECTOR _right_vector = XMVector3Cross(_direction, _up);
			XMVECTOR _strafe = _right_vector * XMVectorGetY(_movement);
			_position_vector += _strafe;
			XMVECTOR _forward = _direction * XMVectorGetX(_movement);
			_position_vector += _forward;
			XMVECTOR _vertical = _up * XMVectorGetZ(_movement);
			_position_vector += _vertical;
			XMFLOAT3 _final_pos = {};
			XMStoreFloat3(&_final_pos, _position_vector);
			_entity->SetPosition(_final_pos);
		}
	}
}
