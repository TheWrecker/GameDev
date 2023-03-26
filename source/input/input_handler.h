
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

	class InputHandler : public IService
	{
	public:
		InputHandler();
		~InputHandler();

		bool Initialize() override;
		void Update() override;
		void Resume();
		void Pause();

	private:
		SystemTicker* ticker;
		Mouse* mouse;
		Keyboard* keyboard;
		Player* player;
		BasicCamera* camera;
		World* world;
		EventHandler* event_handler;
		Scene* scene;
		Overlay* overlay;
		Presenter* presenter;

		float
			mouse_sensitivity,
			rotation_speed;

		bool paused;
	};

#endif // !INPUT_HANDLER_H
