
#include "../gameplay/item_container.h"
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
    inventory = std::make_unique<ItemContainer>(8);
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
        //TODO: gameplay constants? algorithm constants?
        _ray.Advance(0.1f);
        auto _block = world->GetBlock(_ray.end.x, _ray.end.y, _ray.end.z);
        if (_block) //and block is diggable?
        {
            //do other stuff?
            float _dist = 0.0f;
            _block->GetBoundingBox().Intersects(Position_Vector(), DirectX::XMLoadFloat3(&rotation), _dist);
            return _block;
        }
    }
    return nullptr;
}

bool Player::GetPlacementBlockPos(DirectX::XMFLOAT3& pos)
{
    Ray _ray(position, rotation);
    DirectX::XMFLOAT3 _inv_slope = { 1.0f / rotation.x, 1.0f / rotation.y, 1.0f / rotation.z };
    //TODO: gameplay constants/variables?
    while (_ray.GetLength() < 3.0f)
    {
        //TODO: gameplay constants? algorithm constants?
        _ray.Advance(0.1f);
        auto _block = world->GetBlock(_ray.end.x, _ray.end.y, _ray.end.z);
        if (_block)
        {
            //TODO: enhance algorithm?
            pos = _block->Position();
            DirectX::XMFLOAT3 _bbox_min = pos;
            DirectX::XMFLOAT3 _bbox_max = { _bbox_min.x + SOLID_BLOCK_SIZE, _bbox_min.y + SOLID_BLOCK_SIZE, _bbox_min.z + SOLID_BLOCK_SIZE };
            float tmin = 0.0, tmax = INFINITY;

            float tx1 = (_bbox_min.x - _ray.position.x) * _inv_slope.x;
            float tx2 = (_bbox_max.x - _ray.position.x) * _inv_slope.x;

            tmin = min(tx1, tx2);
            tmax = max(tx1, tx2);

            float ty1 = (_bbox_min.y - _ray.position.y) * _inv_slope.y;
            float ty2 = (_bbox_max.y - _ray.position.y) * _inv_slope.y;

            tmin = max(tmin, min(ty1, ty2));
            tmax = min(tmax, max(ty1, ty2));


            float tz1 = (_bbox_min.z - _ray.position.z) * _inv_slope.z;
            float tz2 = (_bbox_max.z - _ray.position.z) * _inv_slope.z;

            tmin = max(tmin, min(tz1, tz2));
            tmax = min(tmax, max(tz1, tz2));

            assert(tmax >= tmin);

            DirectX::XMFLOAT3 _pos = {};
            _pos.x = (tmin * rotation.x) + position.x;
            _pos.y = (tmin * rotation.y) + position.y;
            _pos.z = (tmin * rotation.z) + position.z;

            if (abs(_pos.x - pos.x) < 0.00001f) //left (-x) face
            {
                pos.x -= 1.0f;
                return true;
            }

            if (abs(_pos.x - pos.x) > 0.99990f) // right (+x) face
            {
                pos.x += 1.0f;
                return true;
            }

            if (abs(_pos.y - pos.y) < 0.00001f) // bot (-y) face
            {
                pos.y -= 1.0f;
                return true;
            }

            if (abs(_pos.y - pos.y) > 0.99990f) // top (+y) face
            {
                pos.y += 1.0f;
                return true;
            }

            if (abs(_pos.z - pos.z) < 0.00001f) // front (-z) face
            {
                pos.z -= 1.0f;
                return true;
            }

            if (abs(_pos.z - pos.z) > 0.99990f) // back (+z) face
            {
                pos.z += 1.0f;
                return true;
            }

            /*for (int d = 0; d < 3; ++d) {
                float t1 = (_bbox. ->min[d] - ray->origin[d]) * ray->dir_inv[d];
                float t2 = (box->max[d] - ray->origin[d]) * ray->dir_inv[d];

                tmin = max(tmin, min(t1, t2));
                tmax = min(tmax, max(t1, t2));
            }*/

            //should not reach here
            assert(false);
            return false;
        }
    }
    return false;
}

World* Player::GetWorld()
{
    return world;
}

Scene* Player::GetScene()
{
    return scene;
}

ItemContainer* Player::GetInventory()
{
    return inventory.get();
}
