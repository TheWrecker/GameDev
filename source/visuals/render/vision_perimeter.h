
#ifndef VISION_PERIMETER_H
	#define VISION_PERIMETER_H
	
	#include <vector>

	#include "../utils/frustrum.h"

	class BasicCamera;
	class Segment;
	class Pillar;
	class World;
	class Player;
	class Scene;
	class Presenter;
	class Worldengine;

	class VisionPerimeter
	{
	public:
		VisionPerimeter(Presenter* parent);
		~VisionPerimeter();

		bool Initialize();
		void CollectVisionPerimeter(std::vector<Segment*>& container);
		void PerformViewFrustrumCulling(std::vector<Segment*>& source, std::vector<Segment*>& target);
		void SetPerimeterRange(float value = 70.0f);

	private:
		BasicCamera* camera;
		World* world;
		Player* player;
		Scene* scene;
		Presenter* presenter;
		WorldEngine* world_engine;

		float range;
		Frustrum frustrum;
	};

#endif // !VISION_PERIMETER_H
