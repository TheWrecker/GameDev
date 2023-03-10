
#pragma once

#include <memory>
#include <vector>

#include "defs_world.h"

class SolidBlock;
class Segment;
class Scene;

class World
{
public:
	World(Scene* scene);
	~World();

	void SetupDevelopementWorld();

	Segment* GetSegment(float x, float y);
	SolidBlock* GetBlock(float x, float y, float z);

	bool IsWithinBounds(float x, float y);
	bool IsWithinBounds(unsigned int x, unsigned int y);

private:
	friend class Aggregator;
	friend class SolidBlockRender;
	friend class Overlay;

	Scene* scene;
	Segment* segments[TEMP_WORLD_DIMENSION_SIZE][TEMP_WORLD_DIMENSION_SIZE];
};
