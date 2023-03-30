
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
		Sector(Scene* scene, float x, float z);
		~Sector();

		void AddSegment(Segment* target, SegmentIndex index);
		Segment* CreateSegment(const SegmentIndex& index);
		Segment* GetSegment(const SegmentIndex& index, bool force = false);

		float x, z;
		std::atomic<Segment*> segments[SECTOR_HORIZONTAL_SIZE][SECTOR_VERTICAL_SIZE][SECTOR_HORIZONTAL_SIZE];
		bool biomes_processed;

	private:
		Scene* scene;
		GarbageCollector* garbage_collector;
	};

#endif // !SECTOR_H
