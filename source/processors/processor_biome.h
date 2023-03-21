
#ifndef PROCESSOR_BIOME_H
	#define PROCESSOR_BIOME_H

	#include "defs_world.h"

	class World;

	class BiomeProcessor
	{
	public:

		static void ProcessBiome(World* world, float x, float y, float z);

	};


#endif // !PROCESSOR_BIOME_H
