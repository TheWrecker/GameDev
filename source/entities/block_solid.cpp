
#include "defs_world.h"

#include "block_solid.h"

SolidBlock::SolidBlock(SolidBlockType type, float x, float y, float z)
    :BasicEntity(x, y, z), type(type), 
    collision_box(DirectX::XMFLOAT3(x + SOLID_BLOCK_SIZE / 2.0f, y + SOLID_BLOCK_SIZE / 2.0f, z + SOLID_BLOCK_SIZE / 2.0f),
        DirectX::XMFLOAT3(SOLID_BLOCK_SIZE / 2.0f, SOLID_BLOCK_SIZE / 2.0f, SOLID_BLOCK_SIZE / 2.0f) )
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
    collision_box.Center = DirectX::XMFLOAT3(x, y, z);
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
