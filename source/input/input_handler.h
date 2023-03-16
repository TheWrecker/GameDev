
#ifndef INPUT_HANDLER_H
	#define INPUT_HANDLER_H

	#include "interface_service.h"

	class SystemTicker;
	class Mouse;
	class Keyboard;
	class BasicCamera;
	class EventHandler;
	class Player;
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
		EventHandler* event_handler;
		Scene* scene;
		Overlay* overlay;
		Presenter* presenter;
		Supervisor* supervisor;

		float
			mouse_sensitivity,
			rotation_speed;
	};

#endif // !INPUT_HANDLER_H
