
#pragma once

#include "entity_basic.h"

class TransformableEntity : public BasicEntity
{
public:
	TransformableEntity();
	virtual ~TransformableEntity();

	virtual const DirectX::XMMATRIX Rotation_Matrix() const;
	virtual const DirectX::XMFLOAT3& Rotation() const;
	virtual void SetRotation(float x, float y, float z);
	virtual const DirectX::XMMATRIX Scale_Matrix() const;
	virtual const DirectX::XMFLOAT3& Scale() const;
	virtual void SetScale(float x, float y, float z);
	void SetPosition(float x, float y, float z) override;

protected:
	void UpdateWorldMatrix() override;

	DirectX::XMFLOAT3 scale, rotation;
	DirectX::XMFLOAT4X4 scale_matrix, rotation_matrix;
};