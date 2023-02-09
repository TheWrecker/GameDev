
#include "camera_basic.h"

BasicCamera::BasicCamera(float fieldOfView, float aspectRatio, float near, float far)
{
	SetPosition(0.0f, 0.0f, 0.0f);
	direction = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	SetProperties(fieldOfView, aspectRatio, near, far);
	UpdateViewMatrix();
}

BasicCamera::~BasicCamera()
{
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

void BasicCamera::SetProperties(float fieldOfView, float aspectRatio, float near, float far)
{
	fov = DirectX::XMConvertToRadians(fieldOfView);
	aspect_ratio = aspectRatio;
	near_plane = near;
	far_plane = far;
	UpdateProjectionMatrix();
}

void BasicCamera::SetRotation(float x, float y, float z)
{
	//auto _matrix = DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
	//direction_vector = DirectX::XMVector3TransformNormal(direction_vector, _matrix);
	//direction_vector = DirectX::XMVector2Normalize(direction_vector);
	//up_vector = DirectX::XMVector3TransformNormal(up_vector, _matrix);
	//up_vector = DirectX::XMVector2Normalize(up_vector);
	//right_vector = DirectX::XMVector3TransformNormal(right_vector, _matrix);
	//right_vector = DirectX::XMVector2Normalize(right_vector);
	///*right_vector = DirectX::XMVector3Cross(direction_vector, up_vector);
	//up_vector = DirectX::XMVector3Cross(right_vector, direction_vector);*/

	//UpdateOrientationFloats();
	//TransformableEntity::SetRotation(direction.x, direction.y, direction.z);
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

	UpdateViewMatrix();
	//TransformableEntity::SetRotation(direction.x, direction.y, direction.z);
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

void BasicCamera::UpdateViewProjectionMatrix()
{
	DirectX::XMStoreFloat4x4(&view_projection_matrix, DirectX::XMMatrixMultiply(View_Matrix(), Projection_Matrix()));
}
