
#include "block_solid.h"

SolidBlock::SolidBlock(SolidBlockType type)
    :type(type)
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

unsigned int SolidBlock::GetManagerIndex()
{
    return static_cast<unsigned int>(type);
}
