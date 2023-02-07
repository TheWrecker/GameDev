#include "transformable_entity.h"

TransformableEntity::TransformableEntity()
	:scale(), rotation()
{
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(1.0f, 1.0f, 1.0f);
}

TransformableEntity::~TransformableEntity()
{
}

const DirectX::XMVECTOR& TransformableEntity::GetRotation() const
{
	return rotation_vector;
}

void TransformableEntity::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	rotation_vector = DirectX::XMLoadFloat3(&rotation);
	world_matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYaw(x, y, z), world_matrix);
	DirectX::XMStoreFloat4x4(&world_4x4, world_matrix);
}

const DirectX::XMVECTOR& TransformableEntity::GetScale() const
{
	return scale_vector;
}

void TransformableEntity::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	scale_vector = DirectX::XMLoadFloat3(&scale);
	world_matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(x, y, z), world_matrix);
	DirectX::XMStoreFloat4x4(&world_4x4, world_matrix);
}
