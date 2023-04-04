
#include <DirectXMath.h>

#include "../utils/ray.h"
#include "../entities/player.h"
#include "../scene/camera/camera_basic.h"
#include "../scene/compartments/segment.h"
#include "../scene/compartments/sector.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "../presenter.h"
#include "../gameplay/world_engine.h"

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
	world_engine = presenter->QueryService<WorldEngine*>("world_engine");

	if (!world_engine)
		return false;

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
	if (world_engine->near_sectors.empty())
		return;

	using namespace DirectX;

	//calculate the vision frustrum
	DirectX::XMFLOAT4X4 _proj = {};
	DirectX::XMStoreFloat4x4(&_proj, camera->Projection_Matrix());
	frustrum.CalculateFrustrum(camera->View_Matrix(), _proj);

	//filter all near sectors
	//TODO: render range differs from world near range? make it lesser?
	Segment* _segment = nullptr;
	for (auto _sector : world_engine->near_sectors)
	{
		if (frustrum.IntersectsSector((float)_sector->x, (float)_sector->z))
			{
				for (unsigned int _i = 0; _i < SECTOR_HORIZONTAL_SIZE; _i++)
					for (unsigned int _j = 0; _j < SECTOR_VERTICAL_SIZE; _j++)
						for (unsigned int _k = 0; _k < SECTOR_HORIZONTAL_SIZE; _k++)
						{
							_segment = _sector->segments[_i][_j][_k].load();
							if (_segment)
							{
								XMFLOAT3 _pos = { _segment->Position().x + SEGMENT_LENGTH / 2.0f,
									_segment->Position().y + SEGMENT_LENGTH / 2.0f,
									_segment->Position().z + SEGMENT_LENGTH / 2.0f };

								if (frustrum.IntersectsCube(_pos, SEGMENT_LENGTH / 2.0f))
								{
									container.push_back(_segment);
								}
							}
						}
			}
			
		}

}

void VisionPerimeter::SetPerimeterRange(float value)
{
	range = fmax(value, 20.0f);
}
