
#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

class Ray
{
public:
	Ray(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& dir);
	~Ray();

	void Advance(float distance);

	bool DoesIntersectBox(DirectX::BoundingBox& target, float distance);

	float GetLength();
	const DirectX::XMFLOAT3& GetEnd() const;

private:
	DirectX::XMVECTOR position_vector, direction_vector, end_vector;
	DirectX::XMFLOAT3 position, direction, end;
	float length;
};