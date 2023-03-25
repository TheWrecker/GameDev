
#include "camera_basic.h"

BasicCamera::BasicCamera(float fieldOfView, float aspectRatio, float near, float far)
	:hook_target(nullptr)
{
	position = { 0.0f, 0.0f, 0.0f };
	direction = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	SetProperties(fieldOfView, aspectRatio, near, far);
	UpdateViewMatrix();
}

BasicCamera::~BasicCamera()
{
}

void BasicCamera::FeedRotation(float rotX, float rotY)
{
	using namespace DirectX;
	
	XMVECTOR rotation = { rotX, rotY, 0.0f, 0.0f };
	XMVECTOR rightHand = Right_Vector();
	XMMATRIX pitchMatrix = XMMatrixRotationAxis(rightHand, XMVectorGetY(rotation));
	XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotation));
	Rotate(XMMatrixMultiply(pitchMatrix, yawMatrix));
}

void BasicCamera::FeedMovement(float movX, float movY)
{
	using namespace DirectX;

	XMVECTOR movement = { movX, movY, 0.0f, 0.0f };
	XMVECTOR positionVector = Position_Vector();
	XMVECTOR strafe = Right_Vector() * XMVectorGetX(movement);
	positionVector += strafe;
	XMVECTOR forward = Direction_Vector() * XMVectorGetY(movement);
	positionVector += forward;
	XMStoreFloat3(&position, positionVector);
}

void BasicCamera::UpdateViewMatrix()
{
	DirectX::XMStoreFloat4x4(&view_matrix, DirectX::XMMatrixLookToRH(Position_Vector(), Direction_Vector(), Up_Vector()));
	UpdateViewProjectionMatrix();
}

void BasicCamera::UpdateProjectionMatrix()
{
	DirectX::XMStoreFloat4x4(&projection_matrix, DirectX::XMMatrixPerspectiveFovRH(fov, aspect_ratio, near_plane, far_plane));
	UpdateViewProjectionMatrix();
}

void BasicCamera::Update()
{
	if (hook_target)
		SetPosition(hook_target->Position().x, hook_target->Position().y, hook_target->Position().z);

	UpdateViewMatrix();
}

void BasicCamera::HookToEntity(BasicEntity* target)
{
	hook_target = target;
}

void BasicCamera::SetProperties(float fieldOfView, float aspectRatio, float near, float far)
{
	fov = DirectX::XMConvertToRadians(fieldOfView);
	aspect_ratio = aspectRatio;
	near_plane = near;
	far_plane = far;
	UpdateProjectionMatrix();
}

void BasicCamera::SetDirection(float x, float y, float z)
{
	DirectX::XMFLOAT3 _direction = { x, y, z };
	if ((_direction.x == 0.0f) && (_direction.y == 1.0f) && (_direction.z == 0.0f))
		_direction.y = 0.99f;
	auto _new_direction = DirectX::XMLoadFloat3(&_direction);
	_new_direction = DirectX::XMVector3Normalize(_new_direction);
	_new_direction = DirectX::XMVectorNegate(_new_direction);
	DirectX::XMVECTOR _up_vector = { 0.0f, 1.0f, 0.0f, 0.0f };
	auto _side_vector = DirectX::XMVector3Cross(_up_vector, _new_direction);
	_side_vector = DirectX::XMVector3Normalize(_side_vector);
	_up_vector = DirectX::XMVector3Cross(_new_direction, _side_vector);
	_up_vector = DirectX::XMVector3Normalize(_up_vector);

	DirectX::XMVECTOR _bottom_row = { 0.0f, 0.0f, 0.0f, 1.0f };

	DirectX::XMMATRIX _orientation_matrix = {
		_side_vector,
		_up_vector,
		_new_direction,
		_bottom_row};

	rotation_matrix = _orientation_matrix;
	Rotate(_orientation_matrix);
}

void BasicCamera::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void BasicCamera::Rotate(DirectX::CXMMATRIX matrix)
{
	auto _new_direction = DirectX::XMVector3TransformNormal(Direction_Vector(), matrix);
	_new_direction = DirectX::XMVector3Normalize(_new_direction);
	auto _new_up = DirectX::XMVector3TransformNormal(Up_Vector(), matrix);
	_new_up = DirectX::XMVector3Normalize(_new_up);
	auto _new_right = DirectX::XMVector3TransformNormal(Right_Vector(), matrix);
	_new_right = DirectX::XMVector3Normalize(_new_right);
	/*right_vector = DirectX::XMVector3Cross(direction_vector, up_vector);
	up_vector = DirectX::XMVector3Cross(right_vector, direction_vector);*/

	DirectX::XMStoreFloat3(&direction, _new_direction);
	DirectX::XMStoreFloat3(&up, _new_up);
	DirectX::XMStoreFloat3(&right, _new_right);
}

const DirectX::XMFLOAT3 BasicCamera::Position()
{
	return position;
}

const DirectX::XMFLOAT3 BasicCamera::Direction()
{
	return direction;
}

const DirectX::XMVECTOR BasicCamera::Position_Vector() const
{
	return DirectX::XMLoadFloat3(&position);
}

const DirectX::XMVECTOR BasicCamera::Direction_Vector() const
{
	return DirectX::XMLoadFloat3(&direction);
}

const DirectX::XMVECTOR BasicCamera::Right_Vector() const
{
	return DirectX::XMLoadFloat3(&right);
}

const DirectX::XMVECTOR BasicCamera::Up_Vector() const
{
	return DirectX::XMLoadFloat3(&up);
}

const DirectX::XMMATRIX BasicCamera::View_Matrix() const
{
	return DirectX::XMLoadFloat4x4(&view_matrix);
}

const DirectX::XMMATRIX BasicCamera::Projection_Matrix() const
{
	return DirectX::XMLoadFloat4x4(&projection_matrix);
}

const DirectX::XMMATRIX BasicCamera::View_Projection_Matrix() const
{
	return DirectX::XMLoadFloat4x4(&view_projection_matrix);
}

const DirectX::XMMATRIX BasicCamera::Rotation_Matrix() const
{
	return rotation_matrix;
}

void BasicCamera::UpdateViewProjectionMatrix()
{
	DirectX::XMStoreFloat4x4(&view_projection_matrix, DirectX::XMMatrixMultiply(View_Matrix(), Projection_Matrix()));
}
