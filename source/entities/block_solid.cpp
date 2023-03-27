
#include "defs_world.h"

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

void SolidBlock::SetPosition(float x, float y, float z)
{
    BasicEntity::SetPosition(x, y, z);
}

void SolidBlock::SetPosition(DirectX::XMFLOAT3 pos)
{
    SetPosition(pos.x, pos.y, pos.z);
}

SolidBlockType SolidBlock::GetBlockType()
{
    return type;
}

unsigned int SolidBlock::GetAtlasIndex()
{
    return static_cast<unsigned int>(type);
}
