
#pragma once

#include <functional>
#include <memory>

#include "directxtk/Mouse.h"
#include "platfrom_defs.h"

#include "interface_service.h"

class Mouse : public IService
{
public:
	Mouse(SysWindowHandle handle);
	~Mouse();

	void Update() override;
	void RegisterEvent(std::function<void()> target);

	std::unique_ptr<DirectX::Mouse> mouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> mouse_buttons;
	DirectX::Mouse::State state;

private:
	void DispatchInputs();
};
