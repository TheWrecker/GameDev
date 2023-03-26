
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
		SetPosition(hook_target->Position().x, hook_target->Position().y + eye_height, hook_target->Position().z);

	UpdateViewMatrix();
}

void FirstPersonCamera::SetEyeHeight(float value)
{
	eye_height = value;
}

