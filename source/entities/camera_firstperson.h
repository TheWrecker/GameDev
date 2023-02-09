
#pragma once

#include "camera_basic.h"

class Supervisor;
class SystemTicker;
class Mouse;
class Keyboard;

class FirstPersonCamera : public BasicCamera
{
public:
	FirstPersonCamera(Supervisor* supervisor);
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