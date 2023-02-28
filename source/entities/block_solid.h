
#pragma once

#include <string>

#include "entity_basic.h"

enum SolidBlockType
{
	UNDEFINED = 0,
	TEST = 1,
	DIRT = 2
};

class SolidBlock : public BasicEntity
{
public:
	SolidBlock(SolidBlockType type = SolidBlockType::UNDEFINED);
	~SolidBlock();

	void SetType(SolidBlockType what);
	SolidBlockType GetBlockType();
	unsigned int GetManagerIndex();

private:
	SolidBlockType type;
};
