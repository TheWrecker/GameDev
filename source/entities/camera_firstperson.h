
#pragma once

#include "camera_basic.h"

class SystemTicker;
class Mouse;
class Keyboard;
class Presenter;

class FirstPersonCamera : public BasicCamera
{
public:
	FirstPersonCamera(Presenter* presenter);
	~FirstPersonCamera();

	void Update() override;

private:
	Mouse* mouse;
	Keyboard* keyboard;
	SystemTicker* ticker;

	float
		mouse_sensitivity,
		move_speed,
		rotation_speed;
};