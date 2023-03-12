
#pragma once

#include "entity_transformable.h"
#include "block_solid.h"

class BasicCamera;
class World;
class Scene;

class Player : public TransformableEntity
{
public:
	Player(Scene* scene);
	~Player();

	void Update() override;
	SolidBlock* GetInteractionBlock();

	World* GetWorld();
	Scene* GetScene();

private:
	Scene* scene;
	BasicCamera* camera;
	World* world;
};