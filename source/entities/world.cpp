
#include "segment.h"
#include "pillar.h"

#include "world.h"

World::World(Scene* scene)
	:scene(scene)
{
	
}

World::~World()
{
	for (auto& _element : pillars)
		delete _element.second;
}

void World::SetupDevelopementWorld()
{

	return;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			auto _segment = GetSegment(i * SEGMENT_LENGTH, 0.0f, j * SEGMENT_LENGTH);
			auto _rand = abs((i % 3) + (j % 3));
			switch (_rand)
			{
				case 0:
				{
					_segment->Fill(SolidBlockType::GRASS);
					break;
				}
				case 1:
				{
					_segment->Fill(SolidBlockType::DIRT);
					break;
				}
				case 2:
				{
					_segment->Fill(SolidBlockType::GRASS_ON_DIRT);
					break;
				}
				case 3:
				{
					_segment->Fill(SolidBlockType::STONE);
					break;
				}
				case 4:
				{
					_segment->Fill(SolidBlockType::SAND);
					break;
				}
				default:
					assert(false);
			}
			_segment->RebuildBuffers();
		}

}

SolidBlock* World::CreateBlock(SolidBlockType type, float x, float y, float z, bool rebuildSegment)
{
	SegmentIndex _segment_index = GetSegmentIndex(x, y, z);
	auto _segment = GetSegment(_segment_index, true);

	BlockIndex _block_index = GetBlockIndex(x, y, z);

	auto _block = GetBlock(_segment, _block_index);

	if (_block)
		return _block;

	_segment->AddBlock(type, _block_index.x, _block_index.y, _block_index.z);

	if (rebuildSegment)
		_segment->RebuildBuffers();

	return GetBlock(_segment, _block_index);
}

Segment* World::GetSegment(float x, float y, float z, bool force)
{
	SegmentIndex _index = GetSegmentIndex(x, y, z);
	return GetSegment(_index, force);
}

Segment* World::GetSegment(SegmentIndex& index, bool force)
{
	assert(IsSegmentWithinBounds(index));
	auto _result = pillars.find(PillarIndex(index.x, index.z));
	if (_result != pillars.end())
		return _result->second->GetSegment(index.y, force);
	else if (force)
	{
		auto _pillar = new Pillar(scene, index.x * SEGMENT_LENGTH, index.z * SEGMENT_LENGTH);
		pillars.insert(std::pair(PillarIndex(index.x, index.z), _pillar));
		return _pillar->GetSegment(index.y, true);
	}

	return nullptr;

	/*if (!segments.contains(index))
		segments[index] = new Segment(scene, SolidBlockType::TEST, false, index.x * SEGMENT_LENGTH, index.y * SEGMENT_LENGTH, index.z * SEGMENT_LENGTH);
	return segments[index];*/
}

Segment* World::CreateSegment(float x, float y, float z)
{
	SegmentIndex _index = GetSegmentIndex(x, y, z);
	assert(IsSegmentWithinBounds(_index));
	auto _result = pillars.find(PillarIndex(_index.x, _index.z));
	if (_result != pillars.end())
		return _result->second->GetSegment(_index.y, true);
	else
	{
		auto _pillar = new Pillar(scene, _index.x * SEGMENT_LENGTH, _index.z * SEGMENT_LENGTH);
		pillars.insert(std::pair(PillarIndex(_index.x, _index.z), _pillar));
		return _pillar->CreateSegment(_index.y);
	}
}

SolidBlock* World::GetBlock(float x, float y, float z)
{
	SegmentIndex _segment_index = GetSegmentIndex(x, y, z);
	auto _segment = GetSegment(_segment_index);

	if (!_segment)
		return nullptr;

	BlockIndex _block_index = GetBlockIndex(x, y, z);
	return GetBlock(_segment, _block_index);
}

SolidBlock* World::GetBlock(Segment* segment, BlockIndex& index)
{
	assert(IsBlockWithinBounds(index));
	return segment->blocks[index.x][index.y][index.z];
}

BlockIndex World::GetBlockIndex(float x, float y, float z)
{
	auto _x = fmod(x, SEGMENT_LENGTH);
	auto _y = fmod(y, SEGMENT_LENGTH);
	auto _z = fmod(z, SEGMENT_LENGTH);

	BlockIndex _index = {
	x < 0 ? (unsigned int)floorf(SEGMENT_LENGTH - abs(_x)) : (unsigned int)_x,
	y < 0 ? (unsigned int)floorf(SEGMENT_LENGTH - abs(_y)) : (unsigned int)_y,
	z < 0 ? (unsigned int)floorf(SEGMENT_LENGTH - abs(_z)) : (unsigned int)_z };

	_index.x %= 10;
	_index.y %= 10;
	_index.z %= 10;

	return _index;
}

SegmentIndex World::GetSegmentIndex(float x, float y, float z)
{
	return SegmentIndex{ (int)floor(x / SEGMENT_LENGTH), (int)floor(y / SEGMENT_LENGTH), (int)floor(z / SEGMENT_LENGTH) };
}

bool World::IsSegmentWithinBounds(SegmentIndex index)
{
	assert((index.x < INT32_MAX) && (index.z < INT32_MAX));
	return true;
}

bool World::IsBlockWithinBounds(BlockIndex& index)
{
	assert((index.x < SEGMENT_DIMENSION_SIZE) && (index.y < SEGMENT_DIMENSION_SIZE) && (index.z < SEGMENT_DIMENSION_SIZE));
	return true;
}

BlockIndex::BlockIndex(unsigned int x, unsigned int y, unsigned int z)
	:x(x), y(y), z(z)
{
}

bool operator ==(const BlockIndex& left, const BlockIndex& right) noexcept
{
	return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

SegmentIndex::SegmentIndex(int x, int y, int z)
	:x(x), y(y), z(z)
{
}

bool operator ==(const SegmentIndex& left, const SegmentIndex& right) noexcept
{
	return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

PillarIndex::PillarIndex(int x, int z)
	:x(x),z(z)
{
}

bool operator ==(const PillarIndex& left, const PillarIndex& right) noexcept
{
	return (left.x == right.x) && (left.z == right.z);
}