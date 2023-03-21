
#include "../entities/segment.h"
#include "../entities/world.h"

#include "processor_biome.h"

void BiomeProcessor::ProcessBiome(World* world, float x, float y, float z)
{
	SolidBlockType _type = SolidBlockType::TEST;
	if (y > 0.0f)
	{
		_type = SolidBlockType::GRASS;
	}
	else if (y > -5.0f)
	{
		_type = SolidBlockType::GRASS_ON_DIRT;
	}
	else
	{
		_type = SolidBlockType::SAND;
	}
	world->CreateBlock(_type, x, y, z, false);
}
