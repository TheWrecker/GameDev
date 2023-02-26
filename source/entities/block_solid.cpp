
#include "block_solid.h"

SolidBlock::SolidBlock()
{
    texture_name = "test_checkers";
}

SolidBlock::~SolidBlock()
{
}

void SolidBlock::SetTexture(const std::string& what)
{
    texture_name = what;
}

const std::string& SolidBlock::GetTextureName()
{
    return texture_name;
}
