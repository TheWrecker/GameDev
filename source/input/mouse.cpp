
#ifdef _WINDOWS
#include <Windows.h>
#endif // _WINDOWS

#include "mouse.h"

Mouse::Mouse(SysWindowHandle handle)
{
	mouse = std::make_unique<DirectX::Mouse>();
	mouse->SetWindow(handle);
	mouse_buttons = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

Mouse::~Mouse()
{
}

void Mouse::Update()
{
	auto state = mouse->GetState();
	mouse_buttons->Update(state);
	DispatchInputs();
}

void Mouse::RegisterEvent(std::function<void()> target)
{
}

void Mouse::DispatchInputs()
{
}
