
#ifndef FRUSTRUM_H
	#define FRUSTRUM_H

#include <DirectXMath.h>
#include <DirectXCollision.h>

class Frustrum
{
public:

	Frustrum();
	~Frustrum();

	void CalculateFrustrum(DirectX::CXMMATRIX viewMatrix, DirectX::XMFLOAT4X4& projMatrix, float screenDepth = 1000.0f);
	void CalculateFrustrum(DirectX::XMMATRIX viewProjMatrix);

	bool IntersectsCube(DirectX::XMFLOAT3 center, float radius);

	DirectX::BoundingFrustum frustrum;

private:
	DirectX::XMFLOAT4 planes[6];

};

#endif // !FRUSTRUM_H
