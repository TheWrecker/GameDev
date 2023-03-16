
#ifndef CAMERA_FIRSTPERSON_H
	#define CAMERA_FIRSTPERSON_H

	#include "camera_basic.h"

	class Presenter;

	class FirstPersonCamera : public BasicCamera
	{
	public:
		FirstPersonCamera();
		~FirstPersonCamera();

		void Update() override;
	};

#endif // !CAMERA_FIRSTPERSON_H
