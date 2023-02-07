#include "interface_camera.h"

ICamera::ICamera(float fov, float aspectRatio, float near, float far)
	:fov(fov), aspect_ratio(aspectRatio), near_plane(near), far_plane(far)
{

}

ICamera::~ICamera()
{
}

void ICamera::UpdateViewMatrix()
{
}

void ICamera::UpdateProjectionMatrix()
{
}

void ICamera::SetProperties(float fov, float aspectRatio, float, float)
{
}

void ICamera::SetPosition(float x, float y, float z)
{
}

void ICamera::SetPosition(DirectX::XMFLOAT3& pos)
{
}

void ICamera::SetDirection(float x, float y, float z)
{
}

void ICamera::SetDirection(DirectX::XMFLOAT3& dir)
{
}

void ICamera::Rotate(DirectX::CXMMATRIX matrix)
{
}

void ICamera::Rotate(DirectX::XMFLOAT4X4& matrix)
{
}

const DirectX::XMVECTOR& ICamera::Position() const
{
	// TODO: insert return statement here
}

const DirectX::XMVECTOR& ICamera::Direction() const
{
	// TODO: insert return statement here
}

const DirectX::XMVECTOR& ICamera::Right() const
{
	// TODO: insert return statement here
}

const DirectX::XMVECTOR& ICamera::Up() const
{
	// TODO: insert return statement here
}

const DirectX::XMMATRIX& ICamera::ViewMatrix() const
{
	// TODO: insert return statement here
}

const DirectX::XMMATRIX& ICamera::ProjectionMatrix() const
{
	// TODO: insert return statement here
}

const DirectX::XMMATRIX& ICamera::ViewProjectionMatrix() const
{
	// TODO: insert return statement here
}

void ICamera::UpdateViewProjectionMatrix()
{
}
