
#ifndef PROCESSOR_BIOME_H
	#define PROCESSOR_BIOME_H

	class Sector;
	class World;

	class BiomeProcessor
	{
	public:
		static bool Setup(float heightAmp = 20.0f, float floor = 0.0f);

		static void ProcessBiome(World* world, float x, float y, float z);
		static void ProcessBiome(float* heightmap, World* world, Sector* sector);

	private:
		static float height_amplitude;
		static float generation_floor_height;
	};

#endif // !PROCESSOR_BIOME_H
