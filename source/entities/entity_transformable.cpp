
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

void TransformableEntity::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	DirectX::XMStoreFloat4x4(&rotation_matrix, DirectX::XMMatrixRotationRollPitchYaw(x, y, z));
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
