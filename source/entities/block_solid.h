
#pragma once

#include <string>

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
	SolidBlockType GetBlockType();
	unsigned int GetAtlasIndex();

private:
	SolidBlockType type;
};
