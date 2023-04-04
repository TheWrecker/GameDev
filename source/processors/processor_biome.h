
#ifndef PROCESSOR_BIOME_H
	#define PROCESSOR_BIOME_H

	class Segment;
	class World;

	class BiomeProcessor
	{
	public:
		static bool Setup(float heightAmp = 20.0f, float floor = 0.0f);

		static void ProcessBiome(World* world, int x, float y, int z);
		//static void ProcessBiome(float* heightmap, World* world, Segment* segment);

		static void ProcessSegmentColumn(Segment* segment, unsigned int x, unsigned int z, float value);
		static void FillUnderlyingColumn(Segment* segment, unsigned int x, unsigned int z, unsigned int indexY);

	private:
		static float height_amplitude;
		static float generation_floor_height;
	};

#endif // !PROCESSOR_BIOME_H
