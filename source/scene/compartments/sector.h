
#ifndef SECTOR_H
	#define SECTOR_H

	#include "defs_world.h"

	class Segment;
	class Scene;

	class Sector
	{
	public:
		Sector(Scene* scene, float x, float z);
		~Sector();

		Segment* CreateSegment(const SegmentIndex& index);
		Segment* GetSegment(const SegmentIndex& index, bool force = false);

		float x, z;
		Segment* segments[SECTOR_HORIZONTAL_SIZE][SECTOR_VERTICAL_SIZE][SECTOR_HORIZONTAL_SIZE];
		bool biomes_processed;

	private:
		Scene* scene;
	};

#endif // !SECTOR_H
