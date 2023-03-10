
#pragma once

#include "entity_transformable.h"

class BasicCamera;
class Scene;

class Player : public TransformableEntity
{
public:
	Player(Scene* scene);
	~Player();

private:
	Scene* scene;
	BasicCamera* camera;
};