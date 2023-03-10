
#include "../visuals/utils/ray.h"
#include "camera_basic.h"
#include "block_solid.h"
#include "segment.h"
#include "world.h"
#include "../visuals/scene.h"

#include "player.h"

Player::Player(Scene* scene)
	:scene(scene)
{
	camera = scene->GetActiveCamera();
    world = scene->GetWorld();
}

Player::~Player()
{
}

void Player::Update()
{
	SetPosition(camera->Position());
	//TODO: rotate based on 2D camera direction only?
	SetRotation(camera->Rotation().x, camera->Rotation().y, camera->Rotation().z);

}

void Player::Dig()
{
    Ray _ray(position, rotation);
    while (_ray.GetLength() < 5.0f)
    {
        _ray.Advance(0.1f);
        auto _block = world->GetBlock(_ray.GetEnd().x, _ray.GetEnd().y, _ray.GetEnd().z);
        if (_block)
        {
            auto _segment = world->GetSegment(_ray.GetEnd().x, _ray.GetEnd().z);
            if (!_segment)
                assert(false);
            unsigned int _x = static_cast<unsigned int>(fmod(_ray.GetEnd().x, SEGMENT_LENGTH));
            unsigned int _y = static_cast<unsigned int>(fmod(_ray.GetEnd().y, SEGMENT_LENGTH));
            unsigned int _z = static_cast<unsigned int>(fmod(_ray.GetEnd().z, SEGMENT_LENGTH));
            _segment->RemoveBlock(_x, _y, _z);
            _segment->RebuildBuffers();
            break;
        }
    }
}
