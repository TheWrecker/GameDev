
#include "entity_basic.h"

BasicEntity::BasicEntity(float x, float y, float z)
	:position(), translation_matrix()
{
	SetPosition(x, y, z);
}

BasicEntity::~BasicEntity()
{
}

void BasicEntity::Update()
{
}

const DirectX::XMVECTOR BasicEntity::Position_Vector() const
{
	return DirectX::XMLoadFloat3(&position);
}

DirectX::XMFLOAT3 BasicEntity::Position() const
{
	return position;
}

void BasicEntity::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	DirectX::XMStoreFloat4x4(&translation_matrix, DirectX::XMMatrixTranslation(position.x, position.y, position.z));
	UpdateWorldMatrix();
}

void BasicEntity::SetPosition(DirectX::XMFLOAT3 position)
{
	SetPosition(position.x, position.y, position.z);
}

const DirectX::XMMATRIX BasicEntity::World_Matrix() const
{
	return DirectX::XMLoadFloat4x4(&world_matrix);
}

const DirectX::XMMATRIX BasicEntity::Translation_Matrix() const
{
	return DirectX::XMLoadFloat4x4(&translation_matrix);
}

void BasicEntity::UpdateWorldMatrix()
{
	world_matrix = translation_matrix;
}
