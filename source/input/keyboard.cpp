
#ifdef _WINDOWS
#include <Windows.h>
#endif // _WINDOWS

#include "keyboard.h"

Keyboard::Keyboard()
{
	keyboard = std::make_unique<DirectX::Keyboard>();
	keys = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	state = keyboard->GetState();
}

Keyboard::~Keyboard()
{
}

void Keyboard::Update()
{
	state = keyboard->GetState();
	keys->Update(state);
}

DirectX::Keyboard* Keyboard::GetKeyboard()
{
	return keyboard.get();
}

DirectX::Keyboard::KeyboardStateTracker* Keyboard::GetKeyTracker()
{
	return keys.get();
}

DirectX::Keyboard::State& Keyboard::GetState()
{
	return state;
}
