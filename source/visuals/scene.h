
#pragma once

#include <memory>

#include "interface_drawable.h"

class Presenter;
class BasicCamera;
class TestRender;
class SamplerManager;

enum class SceneMode
{
	DEVELOPEMENT
};

enum class CameraType
{
	UNDEFINED,
	STATIC,
	FIRST_PERSON
};

class Scene : public IDrawable
{
public:
	Scene(Presenter* parent);
	~Scene();

	void SwitchMode(SceneMode mode);
	void Draw() override;
	void Update() override;
	void SwitchCameraType(CameraType type);

	Presenter* GetPresenter();
	BasicCamera* GetActiveCamera();

private:
	Presenter* presenter;
	std::unique_ptr<BasicCamera> active_camera;
	std::unique_ptr<TestRender> test_render;
	std::unique_ptr<SamplerManager> sampler_manager;

	CameraType camera_type;
};
