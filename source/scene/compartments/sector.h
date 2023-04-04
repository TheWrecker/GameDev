
#ifndef SECTOR_H
	#define SECTOR_H
	
	#include <atomic>

	#include "defs_world.h"

	class Segment;
	class Scene;
	class GarbageCollector;

	class Sector
	{
	public:
		Sector(Scene* scene, int x, int z);
		~Sector();

		void AddSegment(Segment* target, SegmentIndex index);
		Segment* CreateSegment(const SegmentIndex& index);
		Segment* GetSegment(const SegmentIndex& index, bool force = false);

		int x, z;
		std::atomic<Segment*> segments[SECTOR_HORIZONTAL_SIZE][SECTOR_VERTICAL_SIZE][SECTOR_HORIZONTAL_SIZE];

	private:
		friend class WorldEngine;

		Scene* scene;
		GarbageCollector* garbage_collector;

		std::atomic<bool> mesh_rebuilt;
		std::atomic<bool> biomes_processed;
	};

#endif // !SECTOR_H
