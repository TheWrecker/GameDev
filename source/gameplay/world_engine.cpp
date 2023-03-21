
#include <cassert>
#include <chrono>

#include "../external/FastNoiseLite/FastNoise.h"

#include "defs_world.h"
#include "../entities/player.h"
#include "../entities/segment.h"
#include "../entities/world.h"
#include "../visuals/scene.h"
#include "../processors/processor_biome.h"

#include "world_engine.h"

WorldEngine::WorldEngine(int targetSeed)
	:scene(nullptr), player(nullptr), world(nullptr), seed(targetSeed)
{
	noise_generator = std::make_unique<FastNoise>();

	if (seed == 0) //no seed passed, calculate one
	{
		auto _time = std::chrono::system_clock::now().time_since_epoch().count();
		if (_time > 0)
			seed = static_cast<int>(_time);
		else
			assert(false); //time since epoch is negative, what do you expect?
	}
	noise_generator->SetSeed(seed);
}

WorldEngine::~WorldEngine()
{
}

void WorldEngine::SetupWorld(Scene* scene)
{
	if (scene)
	{
		this->scene = scene;
		world = scene->GetWorld();
		player = scene->GetPlayer();
	}

	float _bX, _bZ, _val;

	auto _tp1 = std::chrono::high_resolution_clock::now();
	for (int _x = 0; _x < WORLD_INITIAL_DIMENSION_BLOCKS; _x++)
		for (int _z = 0; _z < WORLD_INITIAL_DIMENSION_BLOCKS; _z++)
		{
			_bX = -(WORLD_INITIAL_DIMENSION_BLOCKS / 2.0f) + _x;
			_bZ = -(WORLD_INITIAL_DIMENSION_BLOCKS / 2.0f) + _z;
			_val = WORLD_GENERATION_MAX_HEIGHT_DIFF * noise_generator->GetPerlin(_bX, _bZ);
			BiomeProcessor::ProcessBiome(world, _bX, _val, _bZ);
		}

	//rebuild all world segments' buffers?
	for (auto& _segment : world->segments)
	{
		_segment.second->RebuildBuffers();
	}

	auto _tp2 = std::chrono::high_resolution_clock::now();
	auto _ti = std::chrono::duration_cast<std::chrono::milliseconds>(_tp2 - _tp1).count();
	int i = 0;
}

void WorldEngine::BeginWorldLoading()
{
}

void WorldEngine::WorldLoadTick()
{
}

void WorldEngine::LoadWorld(float x, float z)
{
}

/*_sX = (-(WORLD_INITIAL_DIMENSION_SIZE / 2.0f) + _x) * SEGMENT_LENGTH;
_sZ = (-(WORLD_INITIAL_DIMENSION_SIZE / 2.0f) + _z) * SEGMENT_LENGTH;
_sIndex = _x * WORLD_INITIAL_DIMENSION_SIZE + _z;

auto& _biome = biomes[_sIndex];
_biome.height = WORLD_GENERATION_MAX_HEIGHT_DIFF * noise_generator->GetPerlin(_sX, _sZ);
Segment* _segment = world->GetSegment(_sX, _biome.height, _sZ);
if (!_segment)
assert(false);
_biome.top_segment = _segment;
//TODO: improve
for (int _k = 0; _k < SEGMENT_DIMENSION_SIZE; _k++)
{
	_bX = _sX + _k;
	for (int _l = 0; _l < SEGMENT_DIMENSION_SIZE; _l++)
	{
		_bZ = _sZ + _l;
		auto _val = WORLD_GENERATION_MAX_HEIGHT_DIFF * noise_generator->GetPerlin(_bX, _bZ);
		auto _ind = world->GetBlockIndex(_bX, _val, _bZ);
		_segment->AddBlock(_ind.x, _ind.y, _ind.z);
	}
}
BiomeProcessor::ProcessBiome(_biome);
_segment->RebuildBuffers();
		}*/