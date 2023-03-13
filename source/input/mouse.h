
#ifndef MOUSE_H
	#define MOUSE_H

#include <memory>

	#include "directxtk/Mouse.h"

	#include "interface_service.h"
	#include "defs_platform.h"

	class Mouse : public IService
	{
	public:
		Mouse(SysWindowHandle handle);
		~Mouse();

		void Update() override;
		void ToggleMode();

		DirectX::Mouse* GetMouse();
		DirectX::Mouse::ButtonStateTracker* GetButtonTracker();
		DirectX::Mouse::State& GetState();

	private:
		std::unique_ptr<DirectX::Mouse> mouse;
		std::unique_ptr<DirectX::Mouse::ButtonStateTracker> mouse_buttons;
		DirectX::Mouse::State state;
	};

#endif // !MOUSE_H
