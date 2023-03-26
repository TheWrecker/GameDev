
#include <DirectXMath.h>

#include "../utils/ray.h"
#include "../entities/player.h"
#include "../scene/camera/camera_basic.h"
#include "../scene/compartments/segment.h"
#include "../scene/compartments/pillar.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "../presenter.h"

#include "vision_perimeter.h"

VisionPerimeter::VisionPerimeter(Presenter* parent)
	:presenter(parent), scene(nullptr), camera(nullptr), player(nullptr), world(nullptr), range(50.0f)
{
}

VisionPerimeter::~VisionPerimeter()
{
}

bool VisionPerimeter::Initialize()
{
	scene = presenter->QueryService<Scene*>("scene");

	if (!scene)
		return false;

	player = scene->GetPlayer();
	world = scene->GetWorld();
	camera = scene->GetActiveCamera();

	return player && world && camera;
}

void VisionPerimeter::CollectVisionPerimeter(std::vector<Segment*>& container)
{
	using namespace DirectX;

	//check if the pillar we are on has changed since last update

	//calculate the vision frustrum
	DirectX::XMFLOAT4X4 _proj = {};
	DirectX::XMStoreFloat4x4(&_proj, camera->Projection_Matrix());
	frustrum.CalculateFrustrum(camera->View_Matrix(), _proj);

	//filter all near pillars
	//TODO: render range differs from world near range? make it lesser?
	for (auto& _pillar : world->near_pillars)
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
