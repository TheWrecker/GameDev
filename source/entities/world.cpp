
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

	for (unsigned int i = 0; i < 10; i++)
		for (unsigned int j = 0; j < 10; j++)
		{
			if (segments[i][j])
				delete segments[i][j];
		}
}

void World::SetupDevelopementWorld()
{
	for (unsigned int i = 0; i < 10; i++)
		for (unsigned int j = 0; j < 10; j++)
		{
			segments[i][j] = new Segment(scene, SolidBlockType::GRASS, true, i * SEGMENT_LENGTH, 0.0f, j * SEGMENT_LENGTH);
		}
}
