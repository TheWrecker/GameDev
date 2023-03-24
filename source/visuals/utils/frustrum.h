
#ifndef FRUSTRUM_H
	#define FRUSTRUM_H

#include <DirectXMath.h>
#include <DirectXCollision.h>

class Frustrum
{
public:

	Frustrum();
	~Frustrum();

	void CalculateFrustrum(DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projMatrix);
	void CalculateFrustrum(DirectX::XMMATRIX viewProjMatrix);

	bool IntersectsCube(DirectX::XMVECTOR minPoint, DirectX::XMVECTOR maxPoint);

private:
	//DirectX::XMVECTOR planes[6];
	DirectX::BoundingFrustum frustrum;
};

#endif // !FRUSTRUM_H
