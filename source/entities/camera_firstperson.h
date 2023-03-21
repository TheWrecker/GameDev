
#ifndef CAMERA_FIRSTPERSON_H
	#define CAMERA_FIRSTPERSON_H

	#include "camera_basic.h"

	class SolidBlock;
	class Presenter;

	class FirstPersonCamera : public BasicCamera
	{
	public:
		FirstPersonCamera();
		~FirstPersonCamera();

		void Update() override;
		void SetEyeHeight(float value);

	private:
		float eye_height;
	};

#endif // !CAMERA_FIRSTPERSON_H
