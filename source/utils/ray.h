
#ifndef RAY_H
	#define RAY_H

	#include <DirectXMath.h>
	#include <DirectXCollision.h>

	class Ray
	{
	public:
		Ray(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
		~Ray();

		void Advance(float distance);

		bool DoesIntersectBox(DirectX::BoundingBox& target, float distance);

		float GetLength();

		DirectX::XMFLOAT3 position, direction, end;

	private:
		DirectX::XMVECTOR position_vector, direction_vector, end_vector;
		float length;
	};

#endif // !RAY_H
