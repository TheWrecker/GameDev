
#pragma once

#include <functional>
#include <memory>

#include "directxtk/Keyboard.h"
#include "platfrom_defs.h"

#include "interface_service.h"

class Keyboard : public IService
{
public:
	Keyboard();
	~Keyboard();

	void Update() override;
	void RegisterEvent(std::function<void()> target);

	std::unique_ptr<DirectX::Keyboard> keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> keys;

private:
	void DispatchInputs();


};
