
#pragma once

#include "interface_service.h"

class Mouse;
class Keyboard;
class Player;
class BasicCamera;
class World;
class Scene;
class Overlay;
class Presenter;
class Supervisor;

class InputHandler : public IService
{
public:
	InputHandler(Supervisor* supervisor);
	~InputHandler();

	void Update() override;

private:
	Mouse* mouse;
	Keyboard* keyboard;
	Player* player;
	BasicCamera* camera;
	World* world;
	Scene* scene;
	Overlay* overlay;
	Presenter* presenter;
	Supervisor* supervisor;
};
