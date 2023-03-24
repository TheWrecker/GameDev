
#include "entity_transformable.h"

TransformableEntity::TransformableEntity()
	:scale(), rotation()
{
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(1.0f, 1.0f, 1.0f);
}

TransformableEntity::~TransformableEntity()
{
}

const DirectX::XMMATRIX TransformableEntity::Rotation_Matrix() const
{
	return DirectX::XMLoadFloat4x4(&rotation_matrix);
}

const DirectX::XMFLOAT3& TransformableEntity::Rotation() const
{
	return rotation;
}

DirectX::XMVECTOR TransformableEntity::Rotation_Vector() const
{
	return DirectX::XMLoadFloat3(&rotation);
}

void TransformableEntity::SetRotation(float x, float y, float z)
{
	DirectX::XMFLOAT3 _direction = { x, y, z };
	if ((_direction.x == 0.0f) && (_direction.z == 0.0f))
	{
		if (fabs(_direction.y == 1.0f))
			_direction.y *= 0.99f;
	}
	auto _new_direction = DirectX::XMLoadFloat3(&_direction);
	_new_direction = DirectX::XMVector3Normalize(_new_direction);
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
		_bottom_row };

	DirectX::XMStoreFloat4x4(&rotation_matrix, _orientation_matrix);
	DirectX::XMStoreFloat3(&rotation, _new_direction);
	UpdateWorldMatrix();
}

void TransformableEntity::SetRotation(DirectX::XMMATRIX& matrix, float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	DirectX::XMStoreFloat4x4(&rotation_matrix, matrix);
	UpdateWorldMatrix();
}

const DirectX::XMMATRIX TransformableEntity::Scale_Matrix() const
{
	return DirectX::XMLoadFloat4x4(&scale_matrix);
}

const DirectX::XMFLOAT3& TransformableEntity::Scale() const
{
	return scale;
}

void TransformableEntity::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	DirectX::XMStoreFloat4x4(&scale_matrix, DirectX::XMMatrixScaling(x, y, z));
	UpdateWorldMatrix();
}

void TransformableEntity::SetPosition(float x, float y, float z)
{
	BasicEntity::SetPosition(x, y, z);
	UpdateWorldMatrix();
}

void TransformableEntity::SetPosition(DirectX::XMFLOAT3 position)
{
	BasicEntity::SetPosition(position);
	UpdateWorldMatrix();
}

void TransformableEntity::UpdateWorldMatrix()
{
	auto _scale_rotation_matrix = DirectX::XMMatrixMultiply(Scale_Matrix(), Rotation_Matrix());
	DirectX::XMStoreFloat4x4(&world_matrix, DirectX::XMMatrixMultiply(_scale_rotation_matrix, Translation_Matrix()));
}
