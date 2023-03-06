
#include "block_solid.h"

SolidBlock::SolidBlock(SolidBlockType type, float x, float y, float z)
    :BasicEntity(x, y, z), type(type)
{
}

SolidBlock::~SolidBlock()
{
}

void SolidBlock::SetType(SolidBlockType what)
{
    type = what;
}

SolidBlockType SolidBlock::GetBlockType()
{
    return type;
}

unsigned int SolidBlock::GetAtlasIndex()
{
    return static_cast<unsigned int>(type);
}
