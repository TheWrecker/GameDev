
#include "defs_blocks.h"
#include "../scene/compartments/segment.h"
#include "../scene/compartments/sector.h"
#include "../scene/world.h"

#include "processor_biome.h"

float BiomeProcessor::height_amplitude = 20.0f;
float BiomeProcessor::generation_floor_height = 0.0f;

//TODO: add randomness to sublayers

bool BiomeProcessor::Setup(float heightAmp, float floor)
{
	height_amplitude = heightAmp;
	generation_floor_height = floor;
	return true;
}

void BiomeProcessor::ProcessBiome(World* world, float x, float y, float z)
{
	y *= height_amplitude;
	y += height_amplitude;

	static float _max = 20.0f;
	static float _min = 20.0f;

	_max = fmax(_max, y);
	_min = fmin(_min, y);

	BlockType _type = BlockType::TEST;
	if (y > 20.0f)
	{
		_type = BlockType::GRASS;
	}
	else if (y > 15.0f)
	{
		_type = BlockType::GRASS_ON_DIRT;
	}
	else
	{
		_type = BlockType::SAND;
	}
	world->CreateBlock(_type, x, y, z, false);

	auto _curr = y - SOLID_BLOCK_SIZE;
	auto _inc = SOLID_BLOCK_SIZE;
	while (_curr >= generation_floor_height)
	{
		if (_inc < 4.0f)//create the same block
		{
			world->CreateBlock(_type, x, _curr, z, false);
		}
		else if (_inc < 10.0f) //create dirt
		{
			world->CreateBlock(BlockType::DIRT, x, _curr, z, false);
		}
		else if (_inc < 20.0f) //create stone
		{
			world->CreateBlock(BlockType::STONE, x, _curr, z, false);
		}
		else //create test block for now until we reach the floor
		{
			world->CreateBlock(BlockType::TEST, x, _curr, z, false);
		}

		_inc += SOLID_BLOCK_SIZE;
		_curr -= SOLID_BLOCK_SIZE;
	}
}

void BiomeProcessor::ProcessSegmentColumn(Segment* segment, unsigned int x, unsigned int z, float value)
{
	auto _rand = ((int)(value) ^ 0x93751f) % 10;
	auto _indY = (int)floorf(fmodf(value, SEGMENT_DIMENSION_SIZE));

	if (_indY < _rand)
		_rand = _indY;
	BlockType _type = BlockType::TEST;
	if (value > 20.0f)
	{
		_type = BlockType::GRASS;
	}
	else if (value > 15.0f)
	{
		_type = BlockType::GRASS_ON_DIRT;
	}
	else
	{
		_type = BlockType::SAND;
	}

	segment->SetBlock(x, _indY, z, _type);

	if (_indY == 0)
		return;

	for (int _i = _indY; _i >= _rand; _i--)
	{
		segment->SetBlock(x, _i, z, _type);
	}

	for (int _i = _rand - 1; _i > -1; _i--)
	{
		segment->SetBlock(x, _i, z, BlockType::DIRT);
	}

}

void BiomeProcessor::FillUnderlyingColumn(Segment* segment, unsigned int x, unsigned int z, unsigned int indexY)
{
	switch (indexY)
	{
	case 2:
	{
		for (int _i = SEGMENT_DIMENSION_SIZE - 1; _i > -1; _i--)
		{
			segment->SetBlock(x, _i, z, BlockType::DIRT);
		}
		break;
	}
	case 1:
	{
		for (int _i = SEGMENT_DIMENSION_SIZE - 1; _i > -1; _i--)
		{
			segment->SetBlock(x, _i, z, BlockType::STONE);
		}
		break;
	}
	case 0:
	{
		for (int _i = SEGMENT_DIMENSION_SIZE - 1; _i > -1; _i--)
		{
			segment->SetBlock(x, _i, z, BlockType::TEST);
		}
		break;
	}
	default:
		break;
	}
}
