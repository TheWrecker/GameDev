
#include "basic_entity.h"

BasicEntity::BasicEntity()
	:position(), world_matrix(), world_4x4()
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

const DirectX::XMVECTOR& BasicEntity::GetPosition() const
{
	return position_vector;
}

void BasicEntity::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	position_vector = DirectX::XMLoadFloat3(&position);
	UpdateWorldMatrix();
}

void BasicEntity::UpdateWorldMatrix()
{
	world_matrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMStoreFloat4x4(&world_4x4, world_matrix);
}


