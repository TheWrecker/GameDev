
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
