
#ifndef RAY_H
	#define RAY_H

	#include <DirectXMath.h>
	#include <DirectXCollision.h>

	class Ray
	{
	public:
		Ray(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
		Ray(DirectX::XMVECTOR& pos, DirectX::XMVECTOR& dir);
		~Ray();

		void Advance(float distance);

		bool DoesIntersectBox(DirectX::BoundingBox& target, float distance);

		float GetLength();

		DirectX::XMFLOAT3 position, direction, end;
		DirectX::XMVECTOR position_vector, direction_vector, end_vector;

	private:
		float length;
	};

	inline Ray ComputeIntersectionLine(DirectX::FXMVECTOR p1, DirectX::FXMVECTOR p2);

	inline DirectX::XMVECTOR ComputeIntersectionPoint(DirectX::FXMVECTOR& plane, Ray& ray);

#endif // !RAY_H
