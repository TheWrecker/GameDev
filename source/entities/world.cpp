
#include "segment.h"

#include "world.h"

World::World(Scene* scene)
	:scene(scene)
{
	
}

World::~World()
{
	for (auto& item : segments)
		delete item;
}

void World::SetupDevelopementWorld()
{
	auto segment1 = new Segment(scene, SolidBlockType::TEST, true, 0.0f, 0.0f, 0.0f);
	segments.push_back(segment1);
	auto segment2 = new Segment(scene, SolidBlockType::GRASS, true, 5.0f, 0.0f, 0.0f);
	segment2->RemoveBlock(2, 4, 2);
	segment2->RemoveBlock(2, 3, 2);
	segment2->RemoveBlock(2, 2, 2);
	segment2->RemoveBlock(1, 2, 2);
	segment2->RemoveBlock(3, 2, 2);
	segment2->RemoveBlock(2, 2, 3);
	segment2->RebuildBuffers();
	segments.push_back(segment2);
	auto segment3 = new Segment(scene, SolidBlockType::DIRT, true, 10.0f, 0.0f, 0.0f);
	segments.push_back(segment3);
	auto segment4 = new Segment(scene, SolidBlockType::GRASS, true, 10.0f, 0.0f, 5.0f);
	segments.push_back(segment4);
	auto segment5 = new Segment(scene, SolidBlockType::DIRT, true, 5.0f, 0.0f, 5.0f);
	segments.push_back(segment5);
	auto segment6 = new Segment(scene, SolidBlockType::GRASS, true, 0.0f, 0.0f, 5.0f);
	segments.push_back(segment6);
}
