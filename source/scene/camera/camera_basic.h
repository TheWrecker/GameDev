
#ifndef CAMERA_BASIC_H
	#define CAMERA_BASIC_H

	#include "entity_transformable.h"

	class BasicCamera
	{
	public:
		BasicCamera(float fieldOfView = 90.0f, float aspectRatio = 4.0f/3.0f, float near = 0.001f, float far = 1000.0f);
		~BasicCamera();

		virtual void FeedRotation(float rotX, float rotY);
		virtual void FeedMovement(float movX, float movY);
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix();

		virtual void Update();
		void HookToEntity(BasicEntity* target);

		void SetProperties(float fieldOfView, float aspectRatio, float near, float far);
		void SetPosition(float x, float y, float z);
		void SetDirection(float x, float y, float z);
		void Rotate(DirectX::CXMMATRIX matrix);

		const DirectX::XMFLOAT3 Position();
		const DirectX::XMFLOAT3 Direction();
		const DirectX::XMVECTOR Position_Vector() const;
		const DirectX::XMVECTOR Direction_Vector() const;
		const DirectX::XMVECTOR Right_Vector() const;
		const DirectX::XMVECTOR Up_Vector() const;
		const DirectX::XMMATRIX View_Matrix() const;
		const DirectX::XMMATRIX Projection_Matrix() const;
		const DirectX::XMMATRIX View_Projection_Matrix() const;
		const DirectX::XMMATRIX Rotation_Matrix() const;

	protected:
		friend class Scene;

		virtual void UpdateViewProjectionMatrix();

		BasicEntity* hook_target;

		float
			fov,
			aspect_ratio,
			near_plane,
			far_plane;

		DirectX::XMFLOAT3
			position,
			direction,
			up,
			right;

		DirectX::XMFLOAT4X4
			view_matrix,
			projection_matrix,
			view_projection_matrix;

		DirectX::XMMATRIX rotation_matrix;
	};

#endif // !CAMERA_BASIC_H
