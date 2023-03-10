
#include "../entities/camera_firstperson.h"
#include "../visuals/scene.h"
#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../visuals/presenter.h"
#include "../core/service_manager.h"
#include "../core/supervisor.h"

#include "input_handler.h"

InputHandler::InputHandler(Supervisor* supervisor)
	:supervisor(supervisor), camera(nullptr), world(nullptr), scene(nullptr), keyboard(nullptr), mouse(nullptr), overlay(nullptr)
	,player(nullptr)
{
	presenter = supervisor->Services()->QueryService<Presenter*>("presenter");
	mouse = supervisor->Services()->QueryService<Mouse*>("mouse");
	keyboard = supervisor->Services()->QueryService<Keyboard*>("keyboard");
	overlay = presenter->GetOverlay();
	scene = presenter->GetActiveScene();
	camera = scene->GetActiveCamera();
	world = scene->GetWorld();
	player = scene->GetPlayer();
}

InputHandler::~InputHandler()
{
}

void InputHandler::Update()
{
	//TODO: assignable/dynamic keys
	if (true) //is playing / in-game?
	{
		//camera
		if (camera)
		{
			float _movX, _movY, _rotX, _rotY;
			_movX = _movY = _rotX = _rotY = 0.0f;

			if (keyboard->GetState().W)	_movY = 1.0f;
			if (keyboard->GetState().S)	_movY = -1.0f;
			if (keyboard->GetState().A)	_movX = -1.0f;
			if (keyboard->GetState().D)	_movX = 1.0f;

			if (mouse->GetState().positionMode == DirectX::Mouse::MODE_RELATIVE)
			{
				_rotX = static_cast<float>(-1.0f * (mouse->GetState().x));
				_rotY = static_cast<float>(-1.0f * (mouse->GetState().y));
			}

			camera->FeedInput(_movX, _movY, _rotX, _rotY);
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
			if (mouse->GetButtonTracker()->leftButton == mouse->GetButtonTracker()->HELD)
				int i = 0;
				//player dig event?
		}
	}
}
