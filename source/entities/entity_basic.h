
#pragma once

#include <DirectXMath.h>

class BasicEntity
{
public:
	BasicEntity();
	virtual ~BasicEntity();

	virtual void Render();
	virtual void Update();
	virtual const DirectX::XMVECTOR& Position() const;
	virtual void SetPosition(float x, float y, float z);

protected:
	virtual void UpdateWorldMatrix();

	DirectX::XMFLOAT3 position;
	DirectX::XMVECTOR position_vector;
	DirectX::XMMATRIX world_matrix;
	DirectX::XMFLOAT4X4 world_4x4;
};
