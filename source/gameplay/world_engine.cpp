
#include <cassert>
#include <chrono>
#include <DirectXMath.h>

#include "../external/FastNoiseSIMD/FastNoiseSIMD.h"

#include "defs_world.h"
#include "../core/garbage_collector.h"
#include "../visuals/elements/buffer_index.h"
#include "../entities/player.h"
#include "../scene/compartments/segment.h"
#include "../scene/compartments/sector.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "../processors/processor_biome.h"
#include "../processors/processor_solid_block.h"
#include "../scene/scene.h"
#include "../core/executor.h"
#include "../core/supervisor.h"

#include "world_engine.h"
constexpr auto
	WORLD_INITIAL_DIMENSION_SIZE = 10, //segments in each dimension
	WORLD_INITIAL_DIMENSION_BLOCKS = WORLD_INITIAL_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE,
	WORLD_INITIAL_SIZE = WORLD_INITIAL_DIMENSION_SIZE * WORLD_INITIAL_DIMENSION_SIZE,
	SECTOR_ADJACENT = SEGMENT_LENGTH + SECTOR_WIDTH;

WorldEngine::WorldEngine(int targetSeed)
	:scene(nullptr), player(nullptr), world(nullptr), seed(targetSeed), heightmap(nullptr), last_sector(nullptr), range(3),
	enabled(false), exec_task(nullptr)
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
	noise_generator->SetFrequency(0.02f);
	near_sectors.reserve(50);
}

WorldEngine::~WorldEngine()
{
	noise_generator->FreeNoiseSet(heightmap);
	if (Supervisor::GetLastInstance()->IsExecutorAvailable())
	{
		//executor->RemovePeriodicTask(update_task);
		/*if (exec_task)
			executor->StopExecution(exec_task);*/
	}
}

bool WorldEngine::Initialize()
{
	scene = Supervisor::QueryService<Scene*>("scene");
	executor = Supervisor::QueryService<Executor*>("executor");
	garbage_collector = Supervisor::QueryService<GarbageCollector*>("garbage_collector");

	if (!scene || !executor || !garbage_collector)
		return false;

	world = scene->GetWorld();
	player = scene->GetPlayer();

	if (!world || !player)
		return false;

	auto _t1 = std::chrono::high_resolution_clock::now();
	heightmap = noise_generator->GetPerlinSet(-WORLD_INITIAL_DIMENSION_BLOCKS, -WORLD_INITIAL_DIMENSION_BLOCKS, 0, WORLD_INITIAL_DIMENSION_BLOCKS * 2, WORLD_INITIAL_DIMENSION_BLOCKS * 2, 1);
	auto _t2 = std::chrono::high_resolution_clock::now();
	auto _ti = std::chrono::duration<float>(_t2 - _t1).count() * 1000;
	int i = 0;

	//update_task = executor->RegisterPeriodicTask(std::bind(&WorldEngine::WorldLoadTick, this), 25, false);

	return true;
}


void WorldEngine::Update()
{
	Player* player = scene->GetPlayer();
	if (!player)
		return;

	auto _current_sector = world->GetSectorByWorldPos(player->Position().x, player->Position().z);
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
			auto _sector = world->GetSectorByGridPos(_current_sector->x + (_i * SECTOR_WIDTH), _current_sector->z + (_j * SECTOR_WIDTH), true);
			if (_sector)
				near_sectors.push_back(_sector);
		}
}

void WorldEngine::SetupStartingWorld()
{
	//return;

	if (!scene)
		assert(false);

	world = scene->GetWorld();

	float _val;
	auto _tp1 = std::chrono::high_resolution_clock::now();
	unsigned int _count = 0;
	for (int _x = -WORLD_INITIAL_DIMENSION_BLOCKS; _x < WORLD_INITIAL_DIMENSION_BLOCKS; _x++)
		for (int _z = -WORLD_INITIAL_DIMENSION_BLOCKS; _z < WORLD_INITIAL_DIMENSION_BLOCKS; _z++)
		{
			_val = heightmap[_count];
			BiomeProcessor::ProcessBiome(world, _x, _val, _z);
			_count++;
		}

	//rebuild all world segments' buffers?

	Segment* _segment = nullptr;
	for (auto& _sector : world->sectors)
	{
		for (unsigned int _i = 0; _i < SECTOR_HORIZONTAL_SIZE; _i++)
			for (unsigned int _j = 0; _j < SECTOR_VERTICAL_SIZE; _j++)
				for (unsigned int _k = 0; _k < SECTOR_HORIZONTAL_SIZE; _k++)
				{
					_segment = _sector.second->segments[_i][_j][_k].load();
					if (_segment)
					{
						SolidBlockProcessor::RebuildSegmentInSector(_sector.second, _segment, SegmentIndex(_i, _j, _k),
							_segment->vertex_buffer, _segment->index_buffer);
						_segment->biome_processed = true;
					}
				}

		_sector.second->biomes_processed = true;
	}

	for (int _i = -range; _i < range + 1; _i++)
		for (int _j = -range; _j < range + 1; _j++)
		{
			auto _sector = world->GetSectorByWorldPos(player->Position().x + (_i * SECTOR_WIDTH), player->Position().z + (_j * SECTOR_WIDTH), true);
			near_sectors.push_back(_sector);
		}

	last_sector = world->GetSectorByWorldPos(player->Position().x, player->Position().z);
}

void WorldEngine::StartWorldGeneration()
{
	enabled = true;

	if (exec_task)
	{
		exec_task->ResumeExecution();
		return;
	}

	auto _world_engine_tick = [&]() {
		WorldLoadTick();
		SectorMeshTick();
	};

	exec_task = executor->StartExecution(_world_engine_tick);
}

void WorldEngine::StopWorldGeneration()
{
	enabled = false;
	exec_task->PauseExecution();
}

void WorldEngine::WorldLoadTick()
{
	Player* player = scene->GetPlayer();
	if (!player)
		return;

	if (!enabled)
		return;

	for (auto _sector : near_sectors)
	{
		if (_sector->biomes_processed.load())
			continue;

		unsigned int _count = 0;

		for (unsigned int _i = 0; _i < SECTOR_HORIZONTAL_SIZE; _i++)
			for (unsigned int _k = 0; _k < SECTOR_HORIZONTAL_SIZE; _k++)
			{
				auto _temp_seg = _sector->segments[_i][0][_k].load();

				if (_temp_seg)
				{
					if (_temp_seg->biome_processed)
					{
						_count++;
						continue;
					}
				}

				auto _heightmap = noise_generator->GetPerlinSet(_sector->x + _i * SEGMENT_LENGTH, 
					_sector->z + _k * SEGMENT_LENGTH, 0, SEGMENT_DIMENSION_SIZE, SEGMENT_DIMENSION_SIZE, 1);

				Segment* _segs[5];

				for (unsigned int _t = 0; _t < 5; _t++)
				{
					_segs[_t] = new Segment(scene, BlockType::TEST, false, _sector->x + _i * SEGMENT_LENGTH,
						_t * SEGMENT_LENGTH, _sector->z + _k * SEGMENT_LENGTH);
				}

				for (unsigned int _m = 0; _m < SEGMENT_DIMENSION_SIZE; _m++)
					for (unsigned int _n = 0; _n < SEGMENT_DIMENSION_SIZE; _n++)
					{
						auto _value = _heightmap[_m * SEGMENT_DIMENSION_SIZE + _n] * 20.0f + 20.0f;
						auto _indY = (unsigned int)_value / SEGMENT_DIMENSION_SIZE;

						auto _segment = _segs[_indY];
						BiomeProcessor::ProcessSegmentColumn(_segment, _m, _n, _value);

						for (int _o = _indY - 1; _o > -1; _o--)
						{
							auto _underlying_segment = _segs[_o];
							BiomeProcessor::FillUnderlyingColumn(_underlying_segment, _m, _n, _o);
						}

					}
				

				for (unsigned int _q = 0; _q < 5; _q++)
				{
					if (_segs[_q]->block_count > 0)
					{
						_segs[_q]->RebuildBuffers();
						_segs[_q]->biome_processed.store(true);
						_sector->AddSegment(_segs[_q], SegmentIndex(_i, _q, _k));

					}
					else
					{
						delete _segs[_q];
					}
				}
				
				noise_generator->FreeNoiseSet(_heightmap);
				return;
			}

		if (_count == SECTOR_HORIZONTAL_SIZE * SECTOR_HORIZONTAL_SIZE)
		{
			_sector->biomes_processed.store(true);
		}

	}
	//auto _t2 = std::chrono::high_resolution_clock::now();
	//auto _ti = std::chrono::duration<float>(_t2 - _t1).count() * 1000;
	//if (_ti > 1.5f)
	//	int i = 0;
}

void WorldEngine::SectorMeshTick()
{
	if (!enabled)
		return;

	Segment* _segment = nullptr;

	for (auto _sector : near_sectors)
	{
		if (_sector->mesh_generated.load())
			continue;
	
		for (unsigned int _i = 0; _i < SECTOR_HORIZONTAL_SIZE; _i++)
			for (unsigned int _j = 0; _j < SECTOR_VERTICAL_SIZE; _j++)
				for (unsigned int _k = 0; _k < SECTOR_HORIZONTAL_SIZE; _k++)
				{
					_segment = _sector->segments[_i][_j][_k].load();
					if (_segment)
					{
						if (_segment->mesh_generated.load())
							continue;

						auto _vbuffer = new VertexBuffer<SolidBlockVertex>(scene->GetDevice(), scene->GetContext());
						auto _ibuffer = new IndexBuffer(scene->GetDevice(), scene->GetContext());

						SolidBlockProcessor::RebuildSegmentInSector(_sector, _segment, SegmentIndex(_i, _j, _k), _vbuffer, _ibuffer);

						_segment->draw_mutex.lock();
						
						auto _old_vbuffer = _segment->vertex_buffer.exchange(_vbuffer);
						auto _old_ibuffer = _segment->index_buffer.exchange(_ibuffer);
						_segment->mesh_generated.store(true);

						_segment->draw_mutex.unlock();

						garbage_collector->AddSegmentVertexBuffer(_old_vbuffer);
						garbage_collector->AddIndexBuffer(_old_ibuffer);

						return;

					}
				}

		_sector->mesh_generated.store(true);
	}

	int _x = 0, _z = 0;

	for (auto _sector : near_sectors)
	{
		if (_sector->mesh_rebuilt.load())
			continue;

		_x = _sector->x;
		_z = _sector->z;

		if (!world->GetSectorByGridPos(_x - SEGMENT_LENGTH, _z + SECTOR_ADJACENT))
			continue;
		if (!world->GetSectorByGridPos(_x - SEGMENT_LENGTH, _z))
			continue;
		if (!world->GetSectorByGridPos(_x - SEGMENT_LENGTH, _z - SEGMENT_LENGTH))
			continue;
		if (!world->GetSectorByGridPos(_x , _z + SECTOR_ADJACENT))
			continue;
		if (!world->GetSectorByGridPos(_x , _z - SEGMENT_LENGTH))
			continue;
		if (!world->GetSectorByGridPos(_x + SECTOR_ADJACENT, _z + SECTOR_ADJACENT))
			continue;
		if (!world->GetSectorByGridPos(_x + SECTOR_ADJACENT, _z))
			continue;
		if (!world->GetSectorByGridPos(_x + SECTOR_ADJACENT, _z - SEGMENT_LENGTH))
			continue;

		for (unsigned int _i = 0; _i < SECTOR_HORIZONTAL_SIZE; _i++)
			for (unsigned int _j = 0; _j < SECTOR_VERTICAL_SIZE; _j++)
				for (unsigned int _k = 0; _k < SECTOR_HORIZONTAL_SIZE; _k++)
				{
					_segment = _sector->segments[_i][_j][_k].load();
					if (_segment)
					{
						if (_segment->mesh_rebuilt.load())
							continue;

						auto _vbuffer = new VertexBuffer<SolidBlockVertex>(scene->GetDevice(), scene->GetContext());
						auto _ibuffer = new IndexBuffer(scene->GetDevice(), scene->GetContext());

						SolidBlockProcessor::RebuildSegmentInSector(_sector, _segment, SegmentIndex(_i, _j, _k), _vbuffer, _ibuffer);

						_segment->draw_mutex.lock();

						auto _old_vbuffer = _segment->vertex_buffer.exchange(_vbuffer);
						auto _old_ibuffer = _segment->index_buffer.exchange(_ibuffer);
						_segment->mesh_rebuilt.store(true);

						_segment->draw_mutex.unlock();

						garbage_collector->AddSegmentVertexBuffer(_old_vbuffer);
						garbage_collector->AddIndexBuffer(_old_ibuffer);

						return;

					}
				}

		_sector->mesh_rebuilt.store(true);
	}
}

void WorldEngine::SetVicinityRange(unsigned int value)
{
	value > 1 ? range = value : range = 1;
}

void WorldEngine::LoadWorld(float x, float z)
{
}
