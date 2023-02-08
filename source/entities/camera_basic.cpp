
#include "camera_basic.h"

BasicCamera::BasicCamera(float fieldOfView, float aspectRatio, float near, float far)
{
	SetPosition(0.0f, 0.0f, 0.0f);
	direction = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	direction_vector = DirectX::XMLoadFloat3(&direction);
	up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	up_vector = DirectX::XMLoadFloat3(&up);
	right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	right_vector = DirectX::XMLoadFloat3(&right);
	SetProperties(fieldOfView, aspectRatio, near, far);
	UpdateViewMatrix();
}

BasicCamera::~BasicCamera()
{
}

void BasicCamera::UpdateViewMatrix()
{
	view_matrix = DirectX::XMMatrixLookToRH(Position_Vector(), Direction(), Up());
	DirectX::XMStoreFloat4x4(&view_4x4, view_matrix);
	UpdateViewProjectionMatrix();
}

void BasicCamera::UpdateProjectionMatrix()
{
	projection_matrix = DirectX::XMMatrixPerspectiveFovRH(fov, aspect_ratio, near_plane, far_plane);
	DirectX::XMStoreFloat4x4(&projection_4x4, projection_matrix);
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
	auto _matrix = DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
	direction_vector = DirectX::XMVector3TransformNormal(direction_vector, _matrix);
	direction_vector = DirectX::XMVector2Normalize(direction_vector);
	up_vector = DirectX::XMVector3TransformNormal(up_vector, _matrix);
	up_vector = DirectX::XMVector2Normalize(up_vector);
	right_vector = DirectX::XMVector3TransformNormal(right_vector, _matrix);
	right_vector = DirectX::XMVector2Normalize(right_vector);
	/*right_vector = DirectX::XMVector3Cross(direction_vector, up_vector);
	up_vector = DirectX::XMVector3Cross(right_vector, direction_vector);*/

	UpdateOrientationFloats();
	TransformableEntity::SetRotation(direction.x, direction.y, direction.z);
}

void BasicCamera::Rotate(DirectX::CXMMATRIX matrix)
{
	direction_vector = DirectX::XMVector3TransformNormal(direction_vector, matrix);
	direction_vector = DirectX::XMVector2Normalize(direction_vector);
	up_vector = DirectX::XMVector3TransformNormal(up_vector, matrix);
	up_vector = DirectX::XMVector2Normalize(up_vector);
	right_vector = DirectX::XMVector3TransformNormal(right_vector, matrix);
	right_vector = DirectX::XMVector2Normalize(right_vector);
	/*right_vector = DirectX::XMVector3Cross(direction_vector, up_vector);
	up_vector = DirectX::XMVector3Cross(right_vector, direction_vector);*/

	UpdateOrientationFloats();
	TransformableEntity::SetRotation(direction.x, direction.y, direction.z);
}

const DirectX::XMVECTOR& BasicCamera::Direction() const
{
	return direction_vector;
}

const DirectX::XMVECTOR& BasicCamera::Right() const
{
	return right_vector;
}

const DirectX::XMVECTOR& BasicCamera::Up() const
{
	return up_vector;
}

const DirectX::XMMATRIX& BasicCamera::ViewMatrix() const
{
	return view_matrix;
}

const DirectX::XMMATRIX& BasicCamera::ProjectionMatrix() const
{
	return projection_matrix;
}

const DirectX::XMMATRIX& BasicCamera::ViewProjectionMatrix() const
{
	return view_projection_matrix;
}

void BasicCamera::UpdateOrientationFloats()
{
	DirectX::XMStoreFloat3(&direction, direction_vector);
	DirectX::XMStoreFloat3(&right, right_vector);
	DirectX::XMStoreFloat3(&up, up_vector);
}

void BasicCamera::UpdateViewProjectionMatrix()
{
	view_projection_matrix = DirectX::XMMatrixMultiply(view_matrix,projection_matrix);
	DirectX::XMStoreFloat4x4(&view_projection_4x4, view_projection_matrix);
}
