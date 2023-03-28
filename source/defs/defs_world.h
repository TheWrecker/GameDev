
#ifndef DEFS_WORLD_H
	#define DEFS_WORLD_H

	#include <cmath>

	constexpr int
	SOLID_BLOCK_SIZE = 1,

	SEGMENT_DIMENSION_SIZE = 10,//blocks in each dimension
	SEGMENT_LENGTH = SOLID_BLOCK_SIZE * SEGMENT_DIMENSION_SIZE,

	SECTOR_HORIZONTAL_SIZE = 5, //segments in x,z axis
	SECTOR_VERTICAL_SIZE = 10, //segments in y axis
	SECTOR_WIDTH = SECTOR_HORIZONTAL_SIZE * SEGMENT_LENGTH,
	SECTOR_HEIGHT = SECTOR_VERTICAL_SIZE * SEGMENT_LENGTH,

	WORLD_INITIAL_DIMENSION_SIZE = 20, //segments in each dimension
	WORLD_INITIAL_DIMENSION_BLOCKS = WORLD_INITIAL_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE,
	WORLD_INITIAL_SIZE = WORLD_INITIAL_DIMENSION_SIZE * WORLD_INITIAL_DIMENSION_SIZE;

	struct BlockIndex
	{
		unsigned int x, y, z;

		bool IsValid()
		{
			return (x < SEGMENT_DIMENSION_SIZE) && (y < SEGMENT_DIMENSION_SIZE) && (z < SEGMENT_DIMENSION_SIZE);
		}
	};

	struct SegmentIndex
	{
		unsigned int x, y, z;

		bool IsValid()
		{
			return (x < SECTOR_HORIZONTAL_SIZE) && (y < SECTOR_VERTICAL_SIZE) && (z < SECTOR_HORIZONTAL_SIZE);
		}
	};

	struct SectorIndex
	{
		int x, z;
	};

	inline bool operator ==(const SectorIndex& left, const SectorIndex& right) noexcept
	{
		return (left.x == right.x) && (left.z == right.z);
	}

	inline BlockIndex GetBlockIndex(float x, float y, float z)
	{
		auto _xf = floorf(x);
		auto _yf = floorf(y);
		auto _zf = floorf(z);

		auto _x = ((int)_xf % 10) + 10;
		auto _y = ((int)_yf % 10) + 10;
		auto _z = ((int)_zf % 10) + 10;

		_x %= 10;
		_y %= 10;
		_z %= 10;

		return BlockIndex(_x, _y, _z);
	}

	inline SegmentIndex GetSegmentIndex(float x, float y, float z)
	{
		auto _xf = floorf(x);
		auto _yf = floorf(y);
		auto _zf = floorf(z);

		auto _x = (int)_xf % SECTOR_WIDTH;
		auto _y = (int)_yf % SECTOR_HEIGHT;
		auto _z = (int)_zf % SECTOR_WIDTH;

		_x += SECTOR_WIDTH;
		_y += SECTOR_HEIGHT;
		_z += SECTOR_WIDTH;

		_x %= SECTOR_WIDTH;
		_y %= SECTOR_HEIGHT;
		_z %= SECTOR_WIDTH;

		_x /= SEGMENT_LENGTH;
		_y /= SEGMENT_LENGTH;
		_z /= SEGMENT_LENGTH;

		return SegmentIndex(_x, _y, _z);
	}

	inline SectorIndex GetSectorIndex(float x, float z)
	{
		return SectorIndex{ (int)floor(x / SECTOR_WIDTH), (int)floor(z / SECTOR_WIDTH) };
	}

#endif // !DEFS_WORLD_H
