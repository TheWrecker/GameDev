
#pragma once

#include "transformable_entity.h"

class ICamera : public BasicEntity
{
public:
	ICamera(float fov = 90.0f, float aspectRatio = 4.0f/3.0f, float near = 0.001f, float far = 1000.0f);
	~ICamera();

	virtual void UpdateViewMatrix() = 0;
	virtual void UpdateProjectionMatrix() = 0;

	void SetProperties(float fov, float aspectRatio, float near, float far);
	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3& pos);
	void SetDirection(float x, float y, float z);
	void SetDirection(DirectX::XMFLOAT3& dir);
	void Rotate(DirectX::CXMMATRIX matrix);
	void Rotate(DirectX::XMFLOAT4X4& matrix);

	const DirectX::XMVECTOR& Position() const;
	const DirectX::XMVECTOR& Direction() const;
	const DirectX::XMVECTOR& Right() const;
	const DirectX::XMVECTOR& Up() const;
	const DirectX::XMMATRIX& ViewMatrix() const;
	const DirectX::XMMATRIX& ProjectionMatrix() const;
	const DirectX::XMMATRIX& ViewProjectionMatrix() const;

protected:
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
		position_vector,
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
