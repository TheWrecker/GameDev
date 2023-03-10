
#pragma once

#include <string>
#include <DirectXCollision.h>

#include "entity_basic.h"

enum class SolidBlockType
{
	TEST = 1,
	DIRT = 2,
	GRASS = 3
};

class SolidBlock : public BasicEntity
{
public:
	SolidBlock(SolidBlockType type = SolidBlockType::TEST, float x = 0.0f, float y = 0.0f, float z = 0.0f);
	~SolidBlock();

	void SetType(SolidBlockType what);
	void SetPosition(float x, float y, float z) override;
	void SetPosition(DirectX::XMFLOAT3 pos) override;

	SolidBlockType GetBlockType();
	unsigned int GetAtlasIndex();

private:
	SolidBlockType type;
	DirectX::BoundingBox collision_box;
};
