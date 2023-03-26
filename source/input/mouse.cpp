
#include <cassert>

#include "defs_platform.h"
#include "../core/platform.h"
#include "../core/supervisor.h"
#include "mouse.h"

Mouse::Mouse()
{
	mouse = std::make_unique<DirectX::Mouse>();
	mouse_buttons = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
	state = mouse->GetState();
}

Mouse::~Mouse()
{
}

bool Mouse::Initialize()
{
	auto _handle = Supervisor::QueryService<Platform*>("platform")->GetWindowHandle();
	if (!_handle)
	{
		assert(false);
		return false;
	}
	mouse->SetWindow(_handle);
	return true;
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
