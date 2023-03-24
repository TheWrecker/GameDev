
#include "camera_firstperson.h"

FirstPersonCamera::FirstPersonCamera()
	:eye_height(0.7f)
{
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Update()
{
	if (hook_target)
	{
		position.x = hook_target->Position().x;
		position.y = hook_target->Position().y + eye_height;
		position.z = hook_target->Position().z;
	}

	UpdateViewMatrix();
}

void FirstPersonCamera::SetEyeHeight(float value)
{
	eye_height = value;
}

