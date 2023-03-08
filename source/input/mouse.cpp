
#ifdef _WINDOWS
#include <Windows.h>
#endif // _WINDOWS

#include "mouse.h"

Mouse::Mouse(SysWindowHandle handle)
{
	mouse = std::make_unique<DirectX::Mouse>();
	mouse->SetWindow(handle);
	mouse_buttons = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	state = mouse->GetState();
}

Mouse::~Mouse()
{
}

void Mouse::Update()
{
	state = mouse->GetState();
	mouse_buttons->Update(state);
}

void Mouse::ToggleMode()
{
	if (state.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	}
	else
	{
		mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	}
}

DirectX::Mouse* Mouse::GetMouse()
{
	return mouse.get();
}

DirectX::Mouse::ButtonStateTracker* Mouse::GetButtonTracker()
{
	return mouse_buttons.get();
}

DirectX::Mouse::State& Mouse::GetState()
{
	return state;
}
