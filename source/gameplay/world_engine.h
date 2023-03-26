
#ifndef WORLD_ENGINE_H
	#define WORLD_ENGINE_H

	#include <memory>
	#include <vector>

	#include "../external/FastNoiseSIMD/FastNoiseSIMD.h"

	#include <interface_service.h>
	#include "defs_world.h"

	class Player;
	class Pillar;
	class Segment;
	class World;
	class Scene;

	class WorldEngine : public IService
	{
	public:
		WorldEngine(int seed = 0);
		~WorldEngine();

		bool Initialize() override;
		void SetupStartingWorld();
		void BeginWorldGeneration();
		void WorldLoadTick();

		//std::unique_ptr<FastNoise::Perlin> noise_generator;

	private:


		void LoadWorld(float x, float z);

		Scene* scene;
		World* world;
		Player* player;
		Pillar* last_pillar;
		int seed;

		FastNoiseSIMD* noise_generator;
		float* heightmap;

		std::vector<BiomeInfo> biomes;
	};

#endif // !WORLD_ENGINE_H

