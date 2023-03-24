
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
	:scene(scene)
{
	player = scene->GetPlayer();
	world = scene->GetWorld();
	camera = scene->GetActiveCamera();
}

VisionPerimeter::~VisionPerimeter()
{
}

void VisionPerimeter::CollectVisionPerimeter(std::vector<Segment*>& container)
{
	/*Segment* _segment = nullptr;
	Ray _ray(camera->Position(), camera->Rotation());
	DirectX::XMFLOAT3 _pos = camera->Position();
	DirectX::XMVECTOR _up = { 0.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR _dir = camera->Rotation_Vector();
	DirectX::XMVECTOR _right = DirectX::XMVector3Cross(_dir, _up);
	_right = DirectX::XMVector3Normalize(_right);

	_segment = world->GetSegment(_pos.x, _pos.y, _pos.z);
	if (_segment)
		container.push_back(_segment);

	float _range = 0.8f * SEGMENT_LENGTH;
	unsigned int _i = 1;
	while (_ray.GetLength() < 50.0f)
	{
		_ray.Advance(SEGMENT_LENGTH);
		_segment = world->GetSegment(_ray.end.x, _ray.end.y, _ray.end.z);
		if (_segment)
		{
			container.push_back(_segment);
			if (_segment->IsFull())
			{
				break;
			}

			for (int _j = _range * -_i; _j < _range * _i; _j++)
				for (int _k = _range * -_i; _k < _range * _i; _k++)
			{
					_segment = world->GetSegment(_ray.end.x, _ray.end.y, _ray.end.z);

			}

		}

		_i++;
	}*/
	using namespace DirectX;
	//frustrum.CalculateFrustrum(camera->View_Projection_Matrix());
	 frustrum.CalculateFrustrum(camera->View_Matrix(), camera->Projection_Matrix());
	//brute-force for now
	{
		using namespace DirectX;
		XMVECTOR _segment_length = { 10.0f, 10.0f, 10.0f, 0.0f };

		for (auto& _pillar : world->pillars)
			for (auto& _segment : _pillar.second->segments)
			{
				if (frustrum.IntersectsCube(_segment.second->Position_Vector(), _segment.second->Position_Vector() + _segment_length))
				{
					container.push_back(_segment.second);
				}
			}

	}
}
