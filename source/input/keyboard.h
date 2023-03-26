
#ifndef KEYBOARD_H
	#define KEYBOARD_H

	#include "directxtk/Keyboard.h"

	#include "interface_service.h"

	class Keyboard : public IService
	{
	public:
		Keyboard();
		~Keyboard();

		void Update() override;

		DirectX::Keyboard* GetKeyboard();
		DirectX::Keyboard::KeyboardStateTracker* GetKeyTracker();
		DirectX::Keyboard::State& GetState();

	private:
		std::unique_ptr<DirectX::Keyboard> keyboard;
		std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> keys;
		DirectX::Keyboard::State state;
	};

#endif // !KEYBOARD_H
