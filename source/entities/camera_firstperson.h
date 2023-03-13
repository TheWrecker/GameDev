
#ifndef CAMERA_FIRSTPERSON_H
	#define CAMERA_FIRSTPERSON_H

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

		void FeedInput(float movX, float movY, float rotX, float rotY) override;
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

#endif // !CAMERA_FIRSTPERSON_H
