
#ifndef VISION_PERIMETER_H
	#define VISION_PERIMETER_H
	
	#include <vector>

	#include "../utils/frustrum.h"

	class BasicCamera;
	class Segment;
	class World;
	class Player;
	class Scene;

	class VisionPerimeter
	{
	public:
		VisionPerimeter(Scene* scene);
		~VisionPerimeter();

		void CollectVisionPerimeter(std::vector<Segment*>& container);
		void SetPerimeterRange(float value = 50.0f);

	private:
		BasicCamera* camera;
		World* world;
		Player* player;
		Scene* scene;

		float range;
		Pillar* last_pillar;
		std::vector<Pillar*> near_pillars;

		Frustrum frustrum;
	};

#endif // !VISION_PERIMETER_H
