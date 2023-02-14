
#include "../entities/camera_basic.h"
#include "../entities/camera_firstperson.h"
#include "elements/manager_sampler.h"
#include "presenter.h"
#include "render/test.h"

#include "scene.h"

Scene::Scene(Presenter* parent)
	:presenter(parent), camera_type(CameraType::FIRST_PERSON)
{
	active_camera = std::make_unique<FirstPersonCamera>(parent);
	active_camera->SetPosition(0.0f, 0.0f, 2.0f);
	test_render = std::make_unique<TestRender>(parent, this);
	sampler_manager = std::make_unique<SamplerManager>(parent->GetDevice(), parent->GetContext());
}

Scene::~Scene()
{
}

void Scene::SwitchMode(SceneMode mode)
{
}

void Scene::Draw()
{
	test_render->Draw();
}

void Scene::Update()
{
	active_camera->Update();
}

void Scene::SwitchCameraType(CameraType type)
{
	if (type == camera_type)
		return;
	std::unique_ptr<BasicCamera> _temp;
	switch (type)
	{
	case CameraType::STATIC: 
	{
		_temp = std::make_unique<BasicCamera>();
		_temp->SetProperties(active_camera->fov, active_camera->aspect_ratio, active_camera->near_plane, active_camera->far_plane);
		_temp->SetPosition(active_camera->position.x, active_camera->position.y, active_camera->position.z);
		_temp->SetRotation(active_camera->rotation.x, active_camera->rotation.y, active_camera->rotation.z);
		break;
	}
	case CameraType::FIRST_PERSON:
	{
		_temp = std::make_unique<FirstPersonCamera>(presenter);
		_temp->SetProperties(active_camera->fov, active_camera->aspect_ratio, active_camera->near_plane, active_camera->far_plane);
		_temp->SetPosition(active_camera->position.x, active_camera->position.y, active_camera->position.z);
		_temp->SetRotation(active_camera->rotation.x, active_camera->rotation.y, active_camera->rotation.z);
		break;
	}
	default:
		assert(false);
		break;
	}
	active_camera.swap(_temp);
}

Presenter* Scene::GetPresenter()
{
	return presenter;
}

BasicCamera* Scene::GetActiveCamera()
{
	return active_camera.get();
}
