
#include <DirectXMath.h>

#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../core/sys_ticker.h"
#include "../visuals/presenter.h"

#include "camera_firstperson.h"

FirstPersonCamera::FirstPersonCamera(Presenter* presenter)
	:mouse_sensitivity(100.0f), move_speed(10.0f), rotation_speed(DirectX::XMConvertToRadians(2.0f))
{
	mouse = presenter->QueryService<Mouse*>("mouse");
	keyboard = presenter->QueryService<Keyboard*>("keyboard");
	ticker = presenter->QueryService<SystemTicker*>("ticker");
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::FeedInput(float movX, float movY, float rotX, float rotY)
{
	using namespace DirectX;
	float elapsedTime = ticker->GetLastTickDuration();
	XMVECTOR rotation = { rotX, rotY, 0.0f, 0.0f };
	XMVECTOR movement = { movX, movY, 0.0f, 0.0f };
	rotation = rotation * mouse_sensitivity * rotation_speed * elapsedTime;
	XMVECTOR rightHand = Right_Vector();
	XMMATRIX pitchMatrix = XMMatrixRotationAxis(rightHand, XMVectorGetY(rotation));
	XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotation));
	Rotate(XMMatrixMultiply(pitchMatrix, yawMatrix));

	XMVECTOR positionVector = Position_Vector();
	movement = movement * move_speed * elapsedTime;
	XMVECTOR strafe = Right_Vector() * XMVectorGetX(movement);
	positionVector += strafe;
	XMVECTOR forward = Direction_Vector() * XMVectorGetY(movement);
	positionVector += forward;
	XMStoreFloat3(&position, positionVector);
	Update();
}

void FirstPersonCamera::Update()
{
	UpdateViewMatrix();
}

