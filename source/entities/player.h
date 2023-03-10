
#pragma once

#include "entity_transformable.h"

class BasicCamera;
class World;
class Scene;

class Player : public TransformableEntity
{
public:
	Player(Scene* scene);
	~Player();

	void Update() override;
	void Dig();

private:
	Scene* scene;
	BasicCamera* camera;
	World* world;
};