
#include <random>

#include "../external/FastNoiseLite/FastNoise.h"
#include "../entities/segment.h"
#include "../entities/pillar.h"
#include "../entities/world.h"

#include "processor_biome.h"

bool BiomeProcessor::configured = false;
float BiomeProcessor::height_amplitude = 0.0f;
float BiomeProcessor::generation_floor_height = 0.0f;

//TODO: add randomness to sublayers

void BiomeProcessor::Setup(float heightAmp, float floor)
{
	height_amplitude = heightAmp;
	generation_floor_height = floor;
	configured = true;
}

void BiomeProcessor::ProcessBiome(World* world, float x, float y, float z)
{
	if (!configured)
		Setup();

	y *= height_amplitude;
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

	auto _curr = y - SOLID_BLOCK_SIZE;
	auto _inc = SOLID_BLOCK_SIZE;
	while (_curr > generation_floor_height)
	{
		if (_inc < 4.0f)//create the same block
		{
			world->CreateBlock(_type, x, _curr, z, false);
		}
		else if (_inc < 10.0f) //create dirt
		{
			world->CreateBlock(SolidBlockType::DIRT, x, _curr, z, false);
		}
		else if (_inc < 20.0f) //create stone
		{
			world->CreateBlock(SolidBlockType::STONE, x, _curr, z, false);
		}
		else //create test block for now until we reach the floor
		{
			world->CreateBlock(SolidBlockType::TEST, x, _curr, z, false);
		}

		_inc += SOLID_BLOCK_SIZE;
		_curr -= SOLID_BLOCK_SIZE;
	}
}

void BiomeProcessor::ProcessBiome(FastNoise* noise, World* world, Pillar* pillar)
{
	float _bX, _bZ;
	float _val = 0.0f;
	for (unsigned int _i = 0; _i < SEGMENT_DIMENSION_SIZE; _i++)
		for (unsigned int _j = 0; _j < SEGMENT_DIMENSION_SIZE; _j++)
		{
			_bX = pillar->x + (_i * SOLID_BLOCK_SIZE);
			_bZ = pillar->z + (_j * SOLID_BLOCK_SIZE);
			_val = noise->GetPerlin(_bX, _bZ);
			BiomeProcessor::ProcessBiome(world, _bX, _val, _bZ);
		}
}
