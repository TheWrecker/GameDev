
#include <DirectXMath.h>

#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../core/sys_ticker.h"
#include "../core/service_manager.h"
#include "../core/supervisor.h"

#include "camera_firstperson.h"

FirstPersonCamera::FirstPersonCamera(Supervisor* supervisor)
	:mouse_sensitivity(100.0f), move_speed(2.0f), rotation_speed(DirectX::XMConvertToRadians(2.0f))
{
	mouse = supervisor->Services()->QueryService<Mouse*>("mouse");
	keyboard = supervisor->Services()->QueryService<Keyboard*>("keyboard");
	ticker = supervisor->Services()->QueryService<SystemTicker*>("ticker");
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Update()
{
	DirectX::XMFLOAT2 movementAmount = DirectX::XMFLOAT2(0.0f, 0.0f);
	if (keyboard->GetState().W)	movementAmount.y = 1.0f;
	if (keyboard->GetState().S)	movementAmount.y = -1.0f;
	if (keyboard->GetState().A)	movementAmount.x = -1.0f;
	if (keyboard->GetState().D)	movementAmount.x = 1.0f;

	DirectX::XMFLOAT2 rotationAmount = DirectX::XMFLOAT2(0.0f, 0.0f);
	if (mouse->GetState().positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		rotationAmount.x = -(mouse->GetState().x * mouse_sensitivity);
		rotationAmount.y = -(mouse->GetState().y * mouse_sensitivity);
	}
	mouse->GetMouse()->SetMode(mouse->GetState().leftButton ? DirectX::Mouse::MODE_RELATIVE : DirectX::Mouse::MODE_ABSOLUTE);
	float elapsedTime = ticker->GetLastTickDuration();

	{
		using namespace DirectX;
		XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * rotation_speed * elapsedTime;
		XMVECTOR rightHand = Right_Vector();
		XMMATRIX pitchMatrix = XMMatrixRotationAxis(rightHand, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));
		Rotate(XMMatrixMultiply(pitchMatrix, yawMatrix));

		XMVECTOR positionVector = Position_Vector();
		XMVECTOR movement = XMLoadFloat2(&movementAmount) * move_speed * elapsedTime;
		XMVECTOR strafe = Right_Vector() * XMVectorGetX(movement);
		positionVector += strafe;
		XMVECTOR forward = Direction_Vector() * XMVectorGetY(movement);
		positionVector += forward;
		XMStoreFloat3(&position, positionVector);
	}
	UpdateViewMatrix();
}
