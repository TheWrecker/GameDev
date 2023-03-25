
#ifndef PROCESSOR_BIOME_H
	#define PROCESSOR_BIOME_H

	#include "defs_world.h"

	class FastNoise;
	class World;

	class BiomeProcessor
	{
	public:
		static void Setup(float heightAmp = 20.0f, float floor = -40.0f);

		static void ProcessBiome(World* world, float x, float y, float z);
		static void ProcessBiome(FastNoise* noise, World* world, Pillar* pillar);

	private:
		static bool configured;
		static float height_amplitude;
		static float generation_floor_height;
	};


#endif // !PROCESSOR_BIOME_H
