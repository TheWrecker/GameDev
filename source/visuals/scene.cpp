
#include "../entities/camera_basic.h"
#include "../entities/camera_firstperson.h"
#include "../entities/player.h"
#include "../entities/sun.h"
#include "../entities/world.h"
#include "elements/master_state.h"
#include "elements/master_buffer.h"
#include "elements/manager_model.h"
#include "elements/manager_texture.h"
#include "elements/texture_atlas.h"
#include "../processors/processor_solid_block.h"
#include "presenter.h"
#include "render/aggregator.h"
#include "../gameplay/physics_engine.h"

#include "scene.h"

Scene::Scene(Presenter* parent)
	:presenter(parent), camera_type(CameraType::FIRST_PERSON)
{
	physics_engine = presenter->QueryService<PhysicsEngine*>("physics_engine");
	active_camera = std::make_unique<FirstPersonCamera>();
	active_camera->SetPosition(0.0f, 20.0f, 20.0f);
	active_camera->SetDirection(1.0f, -1.0f, 0.0f);
	sun = std::make_unique<Sun>();
	state_master = std::make_unique<StateMaster>(parent);
	texture_manager = std::make_unique<TextureManager>(parent);
	atlas = std::make_unique<TextureAtlas>(parent);
	model_manager = std::make_unique<ModelManager>();
	buffer_master = std::make_unique<BufferMaster>(this);
	aggregator = std::make_unique<Aggregator>(this);
	world = std::make_unique<World>(this);
	player = std::make_unique<Player>(this);
	player->SetPosition(20.0f, 70.0f, 20.0f);
	active_camera->HookToEntity(player.get());
	//TODO: move to a unit factory?
	physics_engine->RegisterMovementComponent(player.get());
	physics_engine->RegisterCollisionComponent(player.get());
}

Scene::~Scene()
{
}

void Scene::SwitchMode(SceneMode mode)
{
	switch (mode)
	{
		case SceneMode::DEVELOPEMENT:
		{
			SolidBlockProcessor::Setup(atlas.get());
			world->SetupDevelopementWorld();
			break;
		}
		default:
			break;
	}
}

void Scene::Draw()
{
	aggregator->AggregateAllRenders();
}

void Scene::Update()
{
	player->Update();
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
		_temp = std::make_unique<FirstPersonCamera>();
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

ID3D11Device* Scene::GetDevice()
{
	return presenter->GetDevice();
}

ID3D11DeviceContext* Scene::GetContext()
{
	return presenter->GetContext();
}

Aggregator* Scene::GetAggregator()
{
	return aggregator.get();
}

Presenter* Scene::GetPresenter()
{
	return presenter;
}

BasicCamera* Scene::GetActiveCamera()
{
	return active_camera.get();
}

Player* Scene::GetPlayer()
{
	return player.get();
}

Sun* Scene::GetSun()
{
	return sun.get();
}

World* Scene::GetWorld()
{
	return world.get();
}

StateMaster* Scene::GetStateMaster()
{
	return state_master.get();
}

BufferMaster* Scene::GetBufferMaster()
{
	return buffer_master.get();
}

TextureManager* Scene::GetTextureManager()
{
	return texture_manager.get();
}

TextureAtlas* Scene::GetTextureAtlas()
{
	return atlas.get();
}

ModelManager* Scene::GetModelManager()
{
	return model_manager.get();
}
