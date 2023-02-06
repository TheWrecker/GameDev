
#pragma once

#include <memory>

#include "interface_drawable.h"

class Presenter;
//class Camera;

enum class SceneMode
{
	DEVELOPEMENT
};

class Scene : public IDrawable
{
public:
	Scene(Presenter* parent);
	~Scene();

	void SwitchMode(SceneMode mode);
	void Draw() override;
	void Update() override;

private:
	Presenter* presenter;
//	std::unique_ptr<Camera> camera;
};
