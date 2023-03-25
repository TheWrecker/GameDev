
#include <DirectXMath.h>

#include "../visuals/utils/ray.h"
#include "../entities/camera_basic.h"
#include "../entities/segment.h"
#include "../entities/pillar.h"
#include "../entities/player.h"
#include "../entities/world.h"
#include "../scene.h"

#include "vision_perimeter.h"

VisionPerimeter::VisionPerimeter(Scene* scene)
	:scene(scene), range(50.0f)
{
	player = scene->GetPlayer();
	world = scene->GetWorld();
	camera = scene->GetActiveCamera();
	last_pillar = world->GetPillar(player->Position().x, player->Position().z);
}

VisionPerimeter::~VisionPerimeter()
{
}

void VisionPerimeter::CollectVisionPerimeter(std::vector<Segment*>& container)
{
	using namespace DirectX;

	//check if the pillar we are on has changed since last update
	auto _current_pillar = world->GetPillar(player->Position().x, player->Position().z);
	if (_current_pillar != last_pillar)
	{
		//it has changed, we need to reacquire all the pillars in vision perimeter range
		near_pillars.clear();

		//for now brute force
		for (auto& _pillar : world->pillars)
		{
			//calculate the distance between the pillar and player
			float _dist = ((_pillar.second->x - player->Position().x) * (_pillar.second->x - player->Position().x)) +
				((_pillar.second->z - player->Position().z) * (_pillar.second->z - player->Position().z));
			if (_dist < (range * range))
			{
				near_pillars.push_back(_pillar.second);
			}
		}
	}

	//calculate the vision frustrum
	DirectX::XMFLOAT4X4 _proj = {};
	DirectX::XMStoreFloat4x4(&_proj, camera->Projection_Matrix());
	frustrum.CalculateFrustrum(camera->View_Matrix(), _proj);

	//filter all near pillars

	for (auto& _pillar : near_pillars)
	{
		if (frustrum.IntersectsPillar(_pillar->x, _pillar->z))
		{
			for (auto& _segment : _pillar->segments)
			{
				XMFLOAT3 _pos = { _segment.second->Position().x + SEGMENT_LENGTH / 2.0f,
				_segment.second->Position().y + SEGMENT_LENGTH / 2.0f,
				_segment.second->Position().z + SEGMENT_LENGTH / 2.0f };
				if (frustrum.IntersectsCube(_pos, SEGMENT_LENGTH / 2.0f))
				{
					container.push_back(_segment.second);
				}
			}
		}
			
	}

}

void VisionPerimeter::SetPerimeterRange(float value)
{
	range = fmax(value, 20.0f);
}
