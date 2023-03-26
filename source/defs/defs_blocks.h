
#ifndef DEFS_BLOCKS_H
	#define DEFS_BLOCKS_H

	enum class FaceName
	{
		LEFT = 1,
		RIGHT,
		TOP,
		BOTTOM,
		FRONT,
		BACK
	};

	enum class SolidBlockType
	{
		UNDEFINED = 0,
		TEST = 1,
		DIRT = 2,
		GRASS_ON_DIRT = 3,
		GRASS = 4,
		STONE = 5,
		SAND = 6
	};

#endif // !DEFS_BLOCKS_H
