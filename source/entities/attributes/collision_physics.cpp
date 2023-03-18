#include "collision_physics.h"

CollisionPhysics::CollisionPhysics(const DirectX::XMFLOAT3& position, float width, float height)
	:position_ref(position)
{
	if (width > 0.0f)
		half_width = width / 2.0f;
	else
		half_width = 0.0f;

	if (height > 0.0f)
		half_height = height / 2.0f;
	else
		half_height = 0.0f;

	collision_box.Center = position;
	//TODO: different x and z components? (add length component?)
	collision_box.Extents = { half_width, half_height, half_width };
}

CollisionPhysics::~CollisionPhysics()
{
}

void CollisionPhysics::SetCollisionSize(float width, float height)
{
	if (width > 0.0f)
		half_width = width / 2.0f;
	else
		half_width = 0.0f;

	if (height > 0.0f)
		half_height = height / 2.0f;
	else
		half_height = 0.0f;

	collision_box.Extents = { half_width, half_height, half_width };
}

void CollisionPhysics::UpdateCollisionParams()
{
	collision_box.Center = position_ref;
}
