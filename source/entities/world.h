
#pragma once

#include <memory>
#include <vector>

class Segment;
class Scene;

class World
{
public:
	World(Scene* scene);
	~World();

	void SetupDevelopementWorld();

private:
	friend class Aggregator;
	friend class SolidBlockRender;
	friend class Overlay;

	Scene* scene;
	Segment* segments[10][10];
};
