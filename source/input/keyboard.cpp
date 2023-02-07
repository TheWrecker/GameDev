
#ifdef _WINDOWS
#include <Windows.h>
#endif // _WINDOWS

#include "keyboard.h"

Keyboard::Keyboard()
{
	keyboard = std::make_unique<DirectX::Keyboard>();
	keys = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
}

Keyboard::~Keyboard()
{
}

void Keyboard::Update()
{
	auto state = keyboard->GetState();
	keys->Update(state);
	DispatchInputs();
}

void Keyboard::RegisterEvent(std::function<void()> target)
{
}

void Keyboard::DispatchInputs()
{
}
