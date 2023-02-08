
#include "entity_basic.h"

BasicEntity::BasicEntity()
	:position(), translation_matrix()
{
	SetPosition(0.0f, 0.0f, 0.0f);
}

BasicEntity::~BasicEntity()
{
}

void BasicEntity::Render()
{
}

void BasicEntity::Update()
{
}

const DirectX::XMVECTOR BasicEntity::Position_Vector() const
{
	return DirectX::XMLoadFloat3(&position);
}

const DirectX::XMFLOAT3& BasicEntity::Position() const
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

