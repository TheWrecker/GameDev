
#pragma once

#include "entity_transformable.h"

class BasicCamera : public TransformableEntity
{
public:
	BasicCamera(float fieldOfView = 90.0f, float aspectRatio = 4.0f/3.0f, float near = 0.001f, float far = 1000.0f);
	~BasicCamera();

	virtual void UpdateViewMatrix() = 0;
	virtual void UpdateProjectionMatrix() = 0;

	void SetProperties(float fieldOfView, float aspectRatio, float near, float far);
	void SetRotation(float x, float y, float z) override;
	void Rotate(DirectX::CXMMATRIX matrix);

	const DirectX::XMVECTOR& Direction() const;
	const DirectX::XMVECTOR& Right() const;
	const DirectX::XMVECTOR& Up() const;
	const DirectX::XMMATRIX& ViewMatrix() const;
	const DirectX::XMMATRIX& ProjectionMatrix() const;
	const DirectX::XMMATRIX& ViewProjectionMatrix() const;

protected:
	void UpdateOrientationFloats();
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

	DirectX::XMVECTOR
		direction_vector,
		up_vector,
		right_vector;

	DirectX::XMFLOAT4X4
		view_4x4,
		projection_4x4,
		view_projection_4x4;

	DirectX::XMMATRIX
		view_matrix,
		projection_matrix,
		view_projection_matrix;
};
