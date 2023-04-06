
#ifndef FRUSTRUM_H
	#define FRUSTRUM_H

	#include <DirectXMath.h>

	class Frustrum
	{
	public:
		Frustrum();
		~Frustrum();

		void CalculateFrustrum(DirectX::CXMMATRIX viewProjectionMatrix);
		void CalculateFrustrum(DirectX::CXMMATRIX viewMatrix, DirectX::XMFLOAT4X4& projMatrix, float screenDepth = 1000.0f);

		bool IntersectsCube(DirectX::XMFLOAT3 center, float radius);
		bool IntersectsSector(float x, float z);

		const DirectX::XMFLOAT4& Top() const;
		const DirectX::XMFLOAT4& Bottom() const;
		const DirectX::XMFLOAT4& Left() const;
		const DirectX::XMFLOAT4& Right() const;
		const DirectX::XMFLOAT4& Near() const;
		const DirectX::XMFLOAT4& Far() const;

		DirectX::XMVECTOR Top_Vector() const;
		DirectX::XMVECTOR Bottom_Vector() const;
		DirectX::XMVECTOR Left_Vector() const;
		DirectX::XMVECTOR Right_Vector() const;
		DirectX::XMVECTOR Near_Vector() const;
		DirectX::XMVECTOR Far_Vector() const;


	private:
		friend class RenderableFrustrum;

		DirectX::XMFLOAT4 planes[6];
	};

#endif // !FRUSTRUM_H
