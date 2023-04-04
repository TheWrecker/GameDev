
#include "util_funcs.h"
#include "../gameplay/items/item_container.h"
#include "../utils/ray.h"
#include "block_solid.h"
#include "../scene/compartments/segment.h"
#include "../scene/world.h"
#include "../scene/camera/camera_basic.h"
#include "../scene/scene.h"

#include "player.h"

Player::Player(Scene* scene)
    :scene(scene), selected_slot(0), MovePhysics(50, 50.0f), CollisionPhysics(position, 0.8f, 1.8f)
{
	camera = scene->GetActiveCamera();
    world = scene->GetWorld();
    //TODO: inventory capacity?
    inventory = std::make_unique<ItemContainer>(5);
    interaction_mode = InteractionMode::DEFAULT;
}

Player::~Player()
{
}

void Player::Update()
{
	//SetPosition(camera->Position());
    
	//TODO: rotate based on 2D camera direction only? rotate head only?
	SetRotation(camera->Direction().x, camera->Direction().y, camera->Direction().z);
}

void Player::SetInteractionMode(InteractionMode mode)
{
    interaction_mode = mode;
}

void Player::SetActiveInventorySlot(unsigned int slot)
{
    if ((slot < 0) || (slot >= inventory->GetCapacity()))
        return;

    selected_slot = slot;
}

bool Player::GetInteractionBlockPos(DirectX::XMINT3& pos)
{
    Ray _ray(camera->Position(), camera->Direction());
    //TODO: gameplay constants/variables?
    while (_ray.GetLength() < 3.0f)
    {
        //TODO: gameplay constants? algorithm constants?
        _ray.Advance(0.1f);
        auto _block = world->GetBlockByWorldPos(_ray.end.x, _ray.end.y, _ray.end.z);
        if (_block != BlockType::EMPTY) //and block is diggable?
        {
            pos = GetGridPos(_ray.end);
            //do other stuff?
            return true;
        }
    }
    return false;
}

bool Player::GetPlacementBlockPos(DirectX::XMINT3& pos)
{
    Ray _ray(camera->Position(), camera->Direction());
    DirectX::XMFLOAT3 _inv_slope = { 1.0f / camera->Direction().x, 1.0f / camera->Direction().y, 1.0f / camera->Direction().z };
    //TODO: gameplay constants/variables?
    while (_ray.GetLength() < 3.0f)
    {
        //TODO: gameplay constants? algorithm constants?
        _ray.Advance(0.1f);
        auto _block = world->GetBlockByWorldPos(_ray.end.x, _ray.end.y, _ray.end.z);
        if (_block != BlockType::EMPTY)
        {
            //TODO: enhance algorithm?
            pos = GetGridPos(_ray.end);
            DirectX::XMFLOAT3 _bbox_min = { (float)pos.x, (float)pos.y, (float)pos.z };
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
            _pos.x = (tmin * rotation.x) + camera->Position().x;
            _pos.y = (tmin * rotation.y) + camera->Position().y;
            _pos.z = (tmin * rotation.z) + camera->Position().z;

            if (abs(_pos.x - pos.x) < 0.010f) //left (-x) face
            {
                pos.x -= SOLID_BLOCK_SIZE;
                return true;
            }

            if (abs(_pos.x - pos.x) > 0.990f) // right (+x) face
            {
                pos.x += SOLID_BLOCK_SIZE;
                return true;
            }

            if (abs(_pos.y - pos.y) < 0.010f) // bot (-y) face
            {
                pos.y -= SOLID_BLOCK_SIZE;
                return true;
            }

            if (abs(_pos.y - pos.y) > 0.990f) // top (+y) face
            {
                pos.y += SOLID_BLOCK_SIZE;
                return true;
            }

            if (abs(_pos.z - pos.z) < 0.010f) // front (-z) face
            {
                pos.z -= SOLID_BLOCK_SIZE;
                return true;
            }

            if (abs(_pos.z - pos.z) > 0.990f) // back (+z) face
            {
                pos.z += SOLID_BLOCK_SIZE;
                return true;
            }

            /*for (int d = 0; d < 3; ++d) {
                float t1 = (_bbox. ->min[d] - ray->origin[d]) * ray->dir_inv[d];
                float t2 = (box->max[d] - ray->origin[d]) * ray->dir_inv[d];

                tmin = max(tmin, min(t1, t2));
                tmax = min(tmax, max(t1, t2));
            }*/

            //should not reach here
            //assert(false);
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

unsigned int Player::GetActiveInventorySlot()
{
    return selected_slot;
}

InteractionMode Player::GetInteractionMode()
{
    return interaction_mode;
}
