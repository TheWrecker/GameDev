
#ifndef DEFS_WORLD_H
	#define DEFS_WORLD_H

	constexpr unsigned int SEGMENT_DIMENSION_SIZE = 10; //blocks in each dimension
	
	constexpr float
		SOLID_BLOCK_SIZE = 1.0f,
		SEGMENT_LENGTH = SOLID_BLOCK_SIZE * SEGMENT_DIMENSION_SIZE,
		WORLD_GENERATION_MAX_HEIGHT_DIFF = 20.0f;

	constexpr int
		WORLD_INITIAL_DIMENSION_SIZE = 20, //segments in each dimension
		WORLD_INITIAL_DIMENSION_BLOCKS = WORLD_INITIAL_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE,
		WORLD_INITIAL_SIZE = WORLD_INITIAL_DIMENSION_SIZE * WORLD_INITIAL_DIMENSION_SIZE;

	enum class BiomeType
	{
		FOREST,
		VALLEY,
		DESERT
	};

	class Segment;
	struct BiomeInfo
	{
		BiomeType type;
		float height;
		Segment* top_segment;
	};

#endif // !DEFS_WORLD_H