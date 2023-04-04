
#ifndef OVERLAY_H
	#define OVERLAY_H

	#include <d3d11.h>

	#include"defs_platform.h"

	class Player;
	class Sun;
	class Keyboard;
	class Mouse;
	class Aggregator;
	class Scene;
	class Presenter;
	class SystemTicker;
	class WorldEngine;
	class PhysicsEngine;

	class Overlay
	{
	public:
		Overlay(Presenter* parent);
		~Overlay();

		bool Initialize();
		void Draw();
		void Update();

		void Show();
		void Hide();
		void ToggleShow();

	private:
		Player* player;
		Sun* sun;
		Aggregator* aggregator;
		Scene* scene;
		Presenter* presenter;
		SystemTicker* ticker;
		Keyboard* keyboard;
		Mouse* mouse;
		WorldEngine* world_engine;
		PhysicsEngine* physics_engine;

		bool show;
	};

#endif // !OVERLAY_H
