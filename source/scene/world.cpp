
#include <chrono>

#include "../entities/player.h"
#include "compartments/segment.h"
#include "compartments/sector.h"
#include "scene.h"
#include "../processors/processor_biome.h"

#include "world.h"

World::World(Scene* scene)
	:scene(scene)
{
}

World::~World()
{
	for (auto& _element : sectors)
		delete _element.second;
}

void World::SetupDevelopementWorld()
{

	return;

	for (int i = -5; i < 5; i++)
		for (int j = -5; j < 5; j++)
		{
			auto _segment = GetSegment(i * SEGMENT_LENGTH, 0.0f, j * SEGMENT_LENGTH, true);
			auto _rand = abs((i % 3) + (j % 3));
			switch (_rand)
			{
				case 0:
				{
					_segment->Fill(BlockType::GRASS);
					break;
				}
				case 1:
				{
					_segment->Fill(BlockType::DIRT);
					break;
				}
				case 2:
				{
					_segment->Fill(BlockType::GRASS_ON_DIRT);
					break;
				}
				case 3:
				{
					_segment->Fill(BlockType::STONE);
					break;
				}
				case 4:
				{
					_segment->Fill(BlockType::SAND);
					break;
				}
				default:
					assert(false);
			}
		}

	for (auto& _sector : sectors)
	{
		for (unsigned int _x = 0; _x < SECTOR_HORIZONTAL_SIZE; _x++)
			for (unsigned int _y = 0; _y < SECTOR_VERTICAL_SIZE; _y++)
				for (unsigned int _z = 0; _z < SECTOR_HORIZONTAL_SIZE; _z++)
				{
					//if (_sector.second->segments[_x][_y][_z].load())
					//	_sector.second->segments[_x][_y][_z].load()->RebuildBuffers();
				}
	}

}

void World::Update()
{
}

bool World::CreateBlock(BlockType type, float x, float y, float z, bool rebuildSegment)
{
	SectorIndex _sector_index = GetSectorIndex(x, z);
	auto _sector = GetSector(_sector_index, true);

	SegmentIndex _segment_index = GetSegmentIndex(x, y, z);
	auto _segment = GetSegment(_sector, _segment_index, true);

	BlockIndex _block_index = GetBlockIndex(x, y, z);

	auto _block = GetBlock(_segment, _block_index);

	if (_block != BlockType::EMPTY)
		return false;

	_segment->SetBlock(_block_index.x, _block_index.y, _block_index.z, type);

	if (rebuildSegment)
		_segment->RebuildBuffers(_sector, _segment_index);

	return true;
}

Segment* World::GetSegment(float x, float y, float z, bool force)
{
	SectorIndex _sector_index = GetSectorIndex(x, z);
	auto _sector = GetSector(_sector_index, force);

	if (!_sector && force)
	{
		assert(false);
		return nullptr;
	}

	SegmentIndex _index = GetSegmentIndex(x, y, z);
	return GetSegment(_sector, _index, force);
}

Segment* World::GetSegment(Sector* sector, SegmentIndex& index, bool force)
{
	assert(index.IsValid());

	auto _result = sector->segments[index.x][index.y][index.z].load();
	if (_result)
		return _result;
	else if (force)
	{
		auto _segment = new Segment(scene, BlockType::TEST, false, 
			sector->x + (index.x * SEGMENT_LENGTH), (index.y * SEGMENT_LENGTH), sector->z + (index.z * SEGMENT_LENGTH));
		sector->segments[index.x][index.y][index.z].store(_segment);
		return _segment;
	}

	return nullptr;
}

Sector* World::GetSector(float x, float z, bool force)
{
	SectorIndex _index = GetSectorIndex(x, z);
	auto _result = sectors.find(_index);
	if (_result != sectors.end())
		return _result->second;
	else if (force)
	{
		auto _sector = new Sector(scene, _index.x * SECTOR_WIDTH, _index.z * SECTOR_WIDTH);
		sectors.insert(std::pair(_index, _sector));
		return _sector;
	}
	return nullptr;
}

Segment* World::CreateSegment(float x, float y, float z)
{
	SectorIndex _sector_index = GetSectorIndex(x, z);
	SegmentIndex _segment_index = GetSegmentIndex(x, y, z);
	assert(_segment_index.IsValid());

	auto _result = sectors.find(_sector_index);
	if (_result != sectors.end())
		return _result->second->GetSegment(_segment_index, true);
	else
	{
		auto _sector = new Sector(scene, _sector_index.x * SECTOR_WIDTH, _sector_index.z * SECTOR_WIDTH);
		sectors.insert(std::pair(_sector_index, _sector));
		return _sector->CreateSegment(_segment_index);
	}
}

BlockType World::GetBlock(float x, float y, float z)
{
	SectorIndex _sector_index = GetSectorIndex(x, z);
	auto _sector = GetSector(_sector_index);

	if (!_sector)
		return BlockType::EMPTY;

	SegmentIndex _segment_index = GetSegmentIndex(x, y, z);
	if (!_segment_index.IsValid())
		return BlockType::EMPTY;

	auto _segment = GetSegment(_sector,_segment_index);

	if (!_segment)
		return BlockType::EMPTY;

	BlockIndex _block_index = GetBlockIndex(x, y, z);
	return GetBlock(_segment, _block_index);
}

BlockType World::GetBlock(Segment* segment, BlockIndex& index)
{
	assert(index.IsValid());
	return segment->blocks[index.x][index.y][index.z];
}

Sector* World::GetSector(SectorIndex& index, bool force)
{
	auto _result = sectors.find(index);
	if (_result != sectors.end())
		return _result->second;
	else if (force)
	{
		auto _sector = new Sector(scene, index.x * SECTOR_WIDTH, index.z * SECTOR_WIDTH);
		sectors.insert(std::pair(index, _sector));
		return _sector;
	}

	return nullptr;
}
