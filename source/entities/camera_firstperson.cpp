
#include <DirectXMath.h>

#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../core/service_manager.h"
#include "../core/supervisor.h"

#include "camera_firstperson.h"

FirstPersonCamera::FirstPersonCamera(Supervisor* supervisor)
	:mouse_sensitivity(0.1f), move_speed(0.02f), rotation_speed(DirectX::XMConvertToRadians(2.0f))
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
	if (keyboard->state.W)
	{
		movementAmount.y = 1.0f;
	}

	if (keyboard->state.S)
	{
		movementAmount.y = -1.0f;
	}

	if (keyboard->state.A)
	{
		movementAmount.x = -1.0f;
	}

	if (keyboard->state.D)
	{
		movementAmount.x = 1.0f;
	}

	XMFLOAT2 rotationAmount = DirectX::XMFLOAT2(0.0f, 0.0f);

	if (mouse->state.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		rotationAmount.x = -(mouse->state.x * mouse_sensitivity);
		rotationAmount.y = -(mouse->state.y * mouse_sensitivity);
	}

	mouse->mouse->SetMode(mouse->state.leftButton	? DirectX::Mouse::MODE_RELATIVE : DirectX::Mouse::MODE_ABSOLUTE);

	//float elapsedTime = Profiler::lastIterationDuration;
	XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * rotation_speed /** elapsedTime*/;
	XMVECTOR rightHand = Right_Vector();

	XMMATRIX pitchMatrix = XMMatrixRotationAxis(rightHand, XMVectorGetY(rotationVector));
	XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

	Rotate(XMMatrixMultiply(pitchMatrix, yawMatrix));

	XMVECTOR positionVector = Position_Vector();
	XMVECTOR movement = XMLoadFloat2(&movementAmount) * move_speed /** elapsedTime*/;

	XMVECTOR strafe = Right_Vector() * XMVectorGetX(movement);
	positionVector += strafe;

	XMVECTOR forward = Direction_Vector() * XMVectorGetY(movement);
	positionVector += forward;

	XMStoreFloat3(&position, positionVector);

	UpdateViewMatrix();
}
