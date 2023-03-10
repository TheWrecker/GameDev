
#include "segment.h"

#include "world.h"

World::World(Scene* scene)
	:scene(scene)
{
	
}

World::~World()
{
	/*for (auto& item : segments)
		delete item;*/

	for (unsigned int i = 0; i < TEMP_WORLD_DIMENSION_SIZE; i++)
		for (unsigned int j = 0; j < TEMP_WORLD_DIMENSION_SIZE; j++)
		{
			if (segments[i][j])
				delete segments[i][j];
		}
}

void World::SetupDevelopementWorld()
{
	for (unsigned int i = 0; i < TEMP_WORLD_DIMENSION_SIZE; i++)
		for (unsigned int j = 0; j < TEMP_WORLD_DIMENSION_SIZE; j++)
		{
			segments[i][j] = new Segment(scene, SolidBlockType::GRASS, true, i * SEGMENT_LENGTH, 0.0f, j * SEGMENT_LENGTH);
		}
}

//TODO: more efficient lookup/storage? (possibly unordered_map)
//TODO: dynamic world storage (again, unordered_map?)

Segment* World::GetSegment(float x, float z)
{
	unsigned int _x = static_cast<unsigned int>(x / SEGMENT_LENGTH);
	unsigned int _z = static_cast<unsigned int>(z / SEGMENT_LENGTH);
	assert(IsWithinBounds(_x, _z));
	return segments[_x][_z];
}

SolidBlock* World::GetBlock(float x, float y, float z)
{
	unsigned int _x = static_cast<unsigned int>(x / SEGMENT_LENGTH);
	unsigned int _z = static_cast<unsigned int>(z / SEGMENT_LENGTH);

	if (!IsWithinBounds(_x, _z))
		return nullptr;

	unsigned int _y = static_cast<unsigned int>(y / SEGMENT_LENGTH);

	if (_y != 0)
		return nullptr;

	unsigned int _b_x = static_cast<unsigned int>(fmod(x, SEGMENT_LENGTH));
	unsigned int _b_y = static_cast<unsigned int>(fmod(y, SEGMENT_LENGTH));
	unsigned int _b_z = static_cast<unsigned int>(fmod(z, SEGMENT_LENGTH));
	return segments[_x][_z]->blocks[_b_x][_b_y][_b_z];
}

bool World::IsWithinBounds(float x, float z)
{
	unsigned int _x = static_cast<unsigned int>(x / SEGMENT_LENGTH);
	unsigned int _z = static_cast<unsigned int>(z / SEGMENT_LENGTH);

	if ((_x < TEMP_WORLD_DIMENSION_SIZE) && (_x >= 0))
		if ((_z < TEMP_WORLD_DIMENSION_SIZE) && (_z >= 0))
			return true;

	return false;
}

bool World::IsWithinBounds(unsigned int x, unsigned int z)
{
	if ((x < TEMP_WORLD_DIMENSION_SIZE) && (x >= 0))
		if ((z < TEMP_WORLD_DIMENSION_SIZE) && (z >= 0))
			return true;

	return false;
}
