
#include "../core/sys_ticker.h"
#include "../events/begin_placement.h"
#include "../events/finish_dig.h"
#include "../events/event_handler.h"
#include "../scene/camera/camera_firstperson.h"
#include "../entities/player.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../visuals/overlay.h"
#include "../visuals/presenter.h"
#include "../core/supervisor.h"

#include "input_handler.h"

InputHandler::InputHandler()
	:camera(nullptr), world(nullptr), scene(nullptr), keyboard(nullptr), mouse(nullptr), overlay(nullptr),
	player(nullptr), mouse_sensitivity(2.0f), rotation_speed(1.0f), paused(true)
{
}

InputHandler::~InputHandler()
{
}

bool InputHandler::Initialize()
{
	presenter = Supervisor::QueryService<Presenter*>("presenter");
	ticker = Supervisor::QueryService<SystemTicker*>("ticker");
	mouse = Supervisor::QueryService<Mouse*>("mouse");
	keyboard = Supervisor::QueryService<Keyboard*>("keyboard");
	event_handler = Supervisor::QueryService<EventHandler*>("event_handler");
	overlay = presenter->GetOverlay();
	scene = Supervisor::QueryService<Scene*>("scene");
	camera = scene->GetActiveCamera();
	world = scene->GetWorld();
	player = scene->GetPlayer();
	return true;
}

void InputHandler::Update()
{
	if (paused)
		return;

	//TODO: assignable/dynamic keys
	//TODO: issue events whenever possible

	float _elapsed_time = ticker->GetLastTickDuration();

	if (true) //TODO: is playing / in-game? / is game paused?
	{
		//camera
		// we only do rotation here, movement is done by player, which camera is hooked to
		if (camera)
		{
			float _rotX = 0.0f;
			float _rotY = 0.0f;

			if (mouse->GetState().positionMode == DirectX::Mouse::MODE_RELATIVE)
			{
				_rotX = static_cast<float>(-1.0f * (mouse->GetState().x) * _elapsed_time * mouse_sensitivity);
				_rotY = static_cast<float>(-1.0f * (mouse->GetState().y) * _elapsed_time * mouse_sensitivity);
			}

			camera->FeedRotation(_rotX, _rotY);
		}

		//overlay
		if (overlay) //overlay openable?
		{
			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::Escape))
				overlay->ToggleShow();

			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::LeftControl))
				mouse->ToggleMode();
		}

		//player
		if (player) //in-game?
		{
			//world interaction
			switch (player->GetInteractionMode())
			{
				case InteractionMode::DEFAULT:
				{
					break;
				}
				case InteractionMode::DIG_MODE:
				{
					if (mouse->GetButtonTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
					{
						//TODO: hold for 0.4 sec to dig block
						DirectX::XMINT3 _pos = {};
						if (!player->GetInteractionBlockPos(_pos))
							return;

						auto _event = new FinishDigEvent(event_handler, player, _pos, world->GetBlockByGridPos(_pos.x, _pos.y, _pos.z));
						event_handler->FeedEvent(_event);
						break;
					}
					if (mouse->GetButtonTracker()->rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
					{
						DirectX::XMINT3 _pos = {};
						if (!player->GetPlacementBlockPos(_pos))
							return;

						auto _event = new BeginPlacementEvent(event_handler, player, _pos);
						event_handler->FeedEvent(_event);
					}
					break;
				}
				case InteractionMode::PLACEMENT_MODE:
				{
					if (mouse->GetButtonTracker()->rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
					{
						DirectX::XMINT3 _pos = {};
						if (!player->GetPlacementBlockPos(_pos))
							return;

						auto _event = new BeginPlacementEvent(event_handler, player, _pos);
						event_handler->FeedEvent(_event);
					}
					break;
				}
				default:
					break;
			}

			//hud and inventory interaction
			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::D1))
				player->SetActiveInventorySlot(0);

			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::D2))
				player->SetActiveInventorySlot(1);

			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::D3))
				player->SetActiveInventorySlot(2);

			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::D4))
				player->SetActiveInventorySlot(3);

			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::D5))
				player->SetActiveInventorySlot(4);

			if(keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::D9))
				player->SetInteractionMode(InteractionMode::DEFAULT);

			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::D0))
				player->SetInteractionMode(InteractionMode::DIG_MODE);

			if (keyboard->GetKeyTracker()->IsKeyReleased(DirectX::Keyboard::OemMinus))
				player->SetInteractionMode(InteractionMode::PLACEMENT_MODE);

			//movement
			float _mov1 = 0.0f;
			float _mov2 = 0.0f;
			float _mov3 = 0.0f;

			if (keyboard->GetState().W)	_mov1 += 1.0f;
			if (keyboard->GetState().S)	_mov1 += -1.0f;
			if (keyboard->GetState().A)	_mov2 += -1.0f;
			if (keyboard->GetState().D)	_mov2 += 1.0f;

			if (keyboard->GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Space))
				_mov3 = +300.0f;

			player->FeedMovementInfo(_mov1, _mov2, _mov3);
		}
	}
}

void InputHandler::Resume()
{
	paused = false;
}

void InputHandler::Pause()
{
	paused = true;
}
