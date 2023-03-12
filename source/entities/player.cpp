
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

SolidBlock* Player::GetInteractionBlock()
{
    Ray _ray(position, rotation);
    //TODO: gameplay constants/variables?
    while (_ray.GetLength() < 3.0f)
    {
        //TODO: gameplay cosntants? algorithm constants?
        _ray.Advance(0.2f);
        auto _block = world->GetBlock(_ray.GetEnd().x, _ray.GetEnd().y, _ray.GetEnd().z);
        if (_block) //and block is diggable?
        {
            //do other stuff?
            return _block;
        }
    }
    return nullptr;
}

World* Player::GetWorld()
{
    return world;
}

Scene* Player::GetScene()
{
    return scene;
}
