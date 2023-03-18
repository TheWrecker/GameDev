
#ifndef COLLISION_PHYSICS_H
#define COLLISION_PHYSICS_H

#include <DirectXMath.h>
#include <DirectXCollision.h>

class CollisionPhysics
{
public:
	CollisionPhysics(const DirectX::XMFLOAT3& position, float width, float height);
	virtual ~CollisionPhysics();

	void SetCollisionSize(float width, float height);
	void UpdateCollisionParams();

	DirectX::BoundingBox collision_box;

	float
		half_width,
		half_height;

private:
	const DirectX::XMFLOAT3& position_ref;
};


#endif // !COLLISION_PHYSICS_H

