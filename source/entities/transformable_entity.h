
#pragma once

#include "basic_entity.h"

class TransformableEntity : public BasicEntity
{
public:
	TransformableEntity();
	virtual ~TransformableEntity();

	virtual const DirectX::XMVECTOR& GetRotation() const;
	virtual void SetRotation(float x, float y, float z);
	virtual const DirectX::XMVECTOR& GetScale() const;
	virtual void SetScale(float x, float y, float z);

protected:
	DirectX::XMFLOAT3 scale, rotation;
	DirectX::XMVECTOR scale_vector, rotation_vector;
};