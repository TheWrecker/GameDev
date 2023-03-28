
#ifndef FRUSTRUM_H
	#define FRUSTRUM_H

	#include <DirectXMath.h>

	class Frustrum
	{
	public:
		Frustrum();
		~Frustrum();

		void CalculateFrustrum(DirectX::CXMMATRIX viewMatrix, DirectX::XMFLOAT4X4& projMatrix, float screenDepth = 1000.0f);

		bool IntersectsCube(DirectX::XMFLOAT3 center, float radius);
		bool IntersectsSector(float x, float z);

	private:
		DirectX::XMFLOAT4 planes[6];
	};

#endif // !FRUSTRUM_H
