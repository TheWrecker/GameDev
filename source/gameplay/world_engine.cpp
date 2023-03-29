
#include <cassert>
#include <chrono>
#include <DirectXMath.h>

#include "../external/FastNoiseSIMD/FastNoiseSIMD.h"

#include "defs_world.h"
#include "../entities/player.h"
#include "../scene/compartments/segment.h"
#include "../scene/compartments/sector.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "../processors/processor_biome.h"
#include "../scene/scene.h"
#include "../core/executor.h"
#include "../core/supervisor.h"

#include "world_engine.h"

WorldEngine::WorldEngine(int targetSeed)
	:scene(nullptr), player(nullptr), world(nullptr), seed(targetSeed), heightmap(nullptr), last_sector(nullptr), range(2),
	enabled(false)
{
	if (seed == 0) //no seed passed, calculate one
	{
		auto _time = std::chrono::system_clock::now().time_since_epoch().count();
		if (_time > 0)
			seed = static_cast<int>(_time);
		else
			assert(false); //time since epoch is negative, what do you expect?
	}
	noise_generator = std::unique_ptr<FastNoiseSIMD>(FastNoiseSIMD::NewFastNoiseSIMD(seed));
	near_sectors.reserve(25);
}

WorldEngine::~WorldEngine()
{
	noise_generator->FreeNoiseSet(heightmap);
	if (Supervisor::GetLastInstance()->IsExecutorAvailable())
		executor->RemovePeriodicTask(update_task);
}

void WorldEngine::SetupStartingWorld()
{
	//return;

	if (!scene)
		assert(false);

	world = scene->GetWorld();

	float _bX, _bZ, _val;
	auto _tp1 = std::chrono::high_resolution_clock::now();
	for (int _x = 0; _x < WORLD_INITIAL_DIMENSION_BLOCKS; _x++)
		for (int _z = 0; _z < WORLD_INITIAL_DIMENSION_BLOCKS; _z++)
		{
			_bX = -(WORLD_INITIAL_DIMENSION_BLOCKS / 2.0f) + _x;
			_bZ = -(WORLD_INITIAL_DIMENSION_BLOCKS / 2.0f) + _z;
			_val = heightmap[_x * WORLD_INITIAL_DIMENSION_BLOCKS + _z];
			BiomeProcessor::ProcessBiome(world, _bX, _val, _bZ);
		}

	//rebuild all world segments' buffers?

	for (auto& _sector : world->sectors)
	{
		for (unsigned int _i = 0; _i < SECTOR_HORIZONTAL_SIZE; _i++)
			for (unsigned int _j = 0; _j < SECTOR_VERTICAL_SIZE; _j++)
				for (unsigned int _k = 0; _k < SECTOR_HORIZONTAL_SIZE; _k++)
				{
					if (_sector.second->segments[_i][_j][_k])
						_sector.second->segments[_i][_j][_k]->RebuildBuffers();
				}

		_sector.second->biomes_processed = true;
	}

	for (int _i = -range; _i < range + 1; _i++)
		for (int _j = -range; _j < range + 1; _j++)
		{
			auto _sector = world->GetSector(player->Position().x + (_i * SECTOR_WIDTH), player->Position().z + (_j * SECTOR_WIDTH), true);
			near_sectors.push_back(_sector);
		}

	last_sector = world->GetSector(player->Position().x, player->Position().z);
}

bool WorldEngine::Initialize()
{
	scene = Supervisor::QueryService<Scene*>("scene");
	executor = Supervisor::QueryService<Executor*>("executor");

	if (!scene || !executor)
		return false;

	world = scene->GetWorld();
	player = scene->GetPlayer();

	if (!world || !player)
		return false;

	auto _t1 = std::chrono::high_resolution_clock::now();
	heightmap = noise_generator->GetPerlinSet(-(WORLD_INITIAL_DIMENSION_BLOCKS / 2.0f), -(WORLD_INITIAL_DIMENSION_BLOCKS / 2.0f), 0, WORLD_INITIAL_DIMENSION_BLOCKS, WORLD_INITIAL_DIMENSION_BLOCKS, 1);
	auto _t2 = std::chrono::high_resolution_clock::now();
	auto _ti = std::chrono::duration<float>(_t2 - _t1).count() * 1000;
	int i = 0;

	update_task = executor->RegisterPeriodicTask(std::bind(&WorldEngine::WorldLoadTick, this), 50, false);

	return true;
}

void WorldEngine::StartWorldGeneration()
{
	enabled = true;
}

void WorldEngine::StopWorldGeneration()
{
	enabled = false;
}

void WorldEngine::WorldLoadTick()
{
	Player* player = scene->GetPlayer();
	if (!player)
		return;

	auto _current_sector = world->GetSector(player->Position().x, player->Position().z);
	if (_current_sector == last_sector)
		return;

	if (!_current_sector)
		return;

	auto _t1 = std::chrono::high_resolution_clock::now();

	//collect the sectors in immediate vicinity of the player
	near_sectors.clear();

	for (int _i = -range; _i < range + 1; _i++)
		for (int _j = -range; _j < range + 1; _j++)
		{
			auto _sector = world->GetSector(_current_sector->x + (_i * SECTOR_WIDTH), _current_sector->z + (_j * SECTOR_WIDTH), true);
			if (_sector)
				near_sectors.push_back(_sector);
		}

	if (!enabled)
		return;

	for (auto _sector : near_sectors)
	{
		if (_sector->biomes_processed)
			continue;
		unsigned int _count = 0;
		for (unsigned int _i = 0; _i < SECTOR_HORIZONTAL_SIZE; _i++)
			for (unsigned int _k = 0; _k < SECTOR_HORIZONTAL_SIZE; _k++)
			{
				auto _temp_seg = _sector->segments[_i][0][_k];

				if (_temp_seg)
				{
					if (_temp_seg->biome_processed)
					{
						_count++;
						continue;
					}
				}

				auto _heightmap = noise_generator->GetPerlinSet(_sector->x + _i * SEGMENT_LENGTH, _sector->z + _k * SEGMENT_LENGTH, 0,
					SEGMENT_DIMENSION_SIZE, SEGMENT_DIMENSION_SIZE, 1);

				for (unsigned int _t = 0; _t < 6; _t++)
				{
					_sector->CreateSegment(SegmentIndex(_i, _t, _k));
				}

				for (unsigned int _m = 0; _m < SEGMENT_DIMENSION_SIZE; _m++)
					for (unsigned int _n = 0; _n < SEGMENT_DIMENSION_SIZE; _n++)
					{
						auto _value = _heightmap[_m * SEGMENT_DIMENSION_SIZE + _n] * 20.0f + 20.0f;
						auto _indY = (unsigned int)_value / SEGMENT_DIMENSION_SIZE;

						auto _segment = _sector->segments[_i][_indY][_k];
						BiomeProcessor::ProcessSegmentColumn(_segment, _m, _n, _value);

						for (int _o = _indY - 1; _o > -1; _o--)
						{
							auto _underlying_segment = _sector->segments[_i][_o][_k];
							BiomeProcessor::FillUnderlyingColumn(_underlying_segment, _m, _n, _o);
							_underlying_segment->biome_processed = true;
							_underlying_segment->RebuildBuffers();
						}

						_segment->biome_processed = true;
						_segment->RebuildBuffers();
					}

				noise_generator->FreeNoiseSet(_heightmap);
				return;
			}

		if (_count == SECTOR_HORIZONTAL_SIZE * SECTOR_HORIZONTAL_SIZE)
		{
			_sector->biomes_processed = true;
			RebuildSector(_sector);
		}
	}
	auto _t2 = std::chrono::high_resolution_clock::now();
	auto _ti = std::chrono::duration<float>(_t2 - _t1).count() * 1000;
	if (_ti > 1.5f)
		int i = 0;
}

void WorldEngine::RebuildSector(Sector* target)
{
	for (unsigned int _i = 0; _i < SECTOR_HORIZONTAL_SIZE; _i++)
		for (unsigned int _j = 0; _j < SECTOR_VERTICAL_SIZE; _j++)
			for (unsigned int _k = 0; _k < SECTOR_HORIZONTAL_SIZE; _k++)
			{
				if (target->segments[_i][_j][_k])
					target->segments[_i][_j][_k]->RebuildBuffers();
			}

}

void WorldEngine::SetVicinityRange(unsigned int value)
{
	value > 1 ? range = value : range = 1;
}

void WorldEngine::LoadWorld(float x, float z)
{
}
