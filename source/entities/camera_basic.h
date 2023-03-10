
#pragma once

#include "entity_transformable.h"

class BasicCamera : public TransformableEntity
{
public:
	BasicCamera(float fieldOfView = 90.0f, float aspectRatio = 4.0f/3.0f, float near = 0.001f, float far = 1000.0f);
	~BasicCamera();

	virtual void FeedInput(float movX, float movY, float rotX, float rotY);
	virtual void UpdateViewMatrix();
	virtual void UpdateProjectionMatrix();

	void SetProperties(float fieldOfView, float aspectRatio, float near, float far);
	void SetDirection(float x, float y, float z);
	void SetRotation(float x, float y, float z) override;
	void Rotate(DirectX::CXMMATRIX matrix);

	const DirectX::XMVECTOR Direction_Vector() const;
	const DirectX::XMVECTOR Right_Vector() const;
	const DirectX::XMVECTOR Up_Vector() const;
	const DirectX::XMMATRIX View_Matrix() const;
	const DirectX::XMMATRIX Projection_Matrix() const;
	const DirectX::XMMATRIX View_Projection_Matrix() const;

protected:
	friend class Scene;

	virtual void UpdateViewProjectionMatrix();

	float
		fov,
		aspect_ratio,
		near_plane,
		far_plane;

	DirectX::XMFLOAT3
		direction,
		up,
		right;

	DirectX::XMFLOAT4X4
		view_matrix,
		projection_matrix,
		view_projection_matrix;
};
