
#ifndef DEFS_WORLD_H
	#define DEFS_WORLD_H

	constexpr unsigned int
	SEGMENT_DIMENSION_SIZE = 10,
	TEMP_WORLD_DIMENSION_SIZE = 10;
	
	constexpr float
	SOLID_BLOCK_SIZE = 1.0f,
	SEGMENT_LENGTH = SOLID_BLOCK_SIZE * SEGMENT_DIMENSION_SIZE;

#endif // !DEFS_WORLD_H