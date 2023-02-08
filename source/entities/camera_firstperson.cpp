
#include <DirectXMath.h>

#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../core/service_manager.h"
#include "../core/supervisor.h"

#include "camera_firstperson.h"

FirstPersonCamera::FirstPersonCamera(Supervisor* supervisor)
	:mouse_sensitivity(0.1f), move_speed(0.002f), rotation_speed(DirectX::XMConvertToRadians(2.0f))
{
	mouse = supervisor->Services()->QueryService<Mouse*>("mouse");
	keyboard = supervisor->Services()->QueryService<Keyboard*>("keyboard");
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Update()
{
	using namespace DirectX;
	XMFLOAT2 movementAmount = XMFLOAT2(0.0f, 0.0f);
	//TODO: assignable keys
	auto kb = keyboard->keyboard->GetState();

	if (kb.G)
	{
		movementAmount.y = 1.0f;
	}

	if (kb.S)
	{
		movementAmount.y = -1.0f;
	}

	if (kb.A)
	{
		movementAmount.x = -1.0f;
	}

	if (kb.D)
	{
		movementAmount.x = 1.0f;
	}

	XMFLOAT2 rotationAmount = DirectX::XMFLOAT2(0.0f, 0.0f);
	//if (Input::Mouse::leftClick)
	//{
	//	rotationAmount.x = -(Input::Mouse::changeX * mouseSensitivity);
	//	rotationAmount.y = -(Input::Mouse::changeY * mouseSensitivity);
	//}

	//float elapsedTime = Profiler::lastIterationDuration;
	//XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * rotationSpeed /** elapsedTime*/;
	//XMVECTOR rightHand = XMLoadFloat3(&right);

	//XMMATRIX pitchMatrix = XMMatrixRotationAxis(rightHand, XMVectorGetY(rotationVector));
	//XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

	//Rotate(XMMatrixMultiply(pitchMatrix, yawMatrix));

	XMVECTOR positionVector = XMLoadFloat3(&position);
	XMVECTOR movement = XMLoadFloat2(&movementAmount) * move_speed /** elapsedTime*/;

	XMVECTOR strafe = right_vector * XMVectorGetX(movement);
	positionVector += strafe;

	XMVECTOR forward = XMLoadFloat3(&direction) * XMVectorGetY(movement);
	positionVector += forward;

	XMStoreFloat3(&position, positionVector);

	UpdateViewMatrix();
}
