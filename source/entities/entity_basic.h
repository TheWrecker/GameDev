
#pragma once

#include <DirectXMath.h>

class BasicEntity
{
public:
	BasicEntity();
	virtual ~BasicEntity();

	virtual void Render();
	virtual void Update();
	virtual void SetPosition(float x, float y, float z);

	virtual const DirectX::XMVECTOR Position_Vector() const;
	virtual const DirectX::XMFLOAT3& Position() const;
	virtual const DirectX::XMMATRIX World_Matrix() const;
	const DirectX::XMMATRIX Translation_Matrix() const;

protected:
	virtual void UpdateWorldMatrix();

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4X4 translation_matrix, world_matrix;
};
