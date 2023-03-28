
#include "assets/master_state.h"
#include "assets/master_buffer.h"
#include "assets/manager_model.h"
#include "assets/manager_texture.h"
#include "assets/texture_atlas.h"
#include "camera/camera_firstperson.h"
#include "../entities/player.h"
#include "elements/sun.h"
#include "world.h"
#include "../visuals/presenter.h"
#include "../gameplay/physics_engine.h"
#include "../gameplay/world_engine.h"
#include "../events/event_handler.h"

#include "scene.h"

Scene::Scene(Presenter* parent)
	:presenter(parent)
{
	//assets
	texture_manager = std::make_unique<TextureManager>();
	atlas = std::make_unique<TextureAtlas>();
	model_manager = std::make_unique<ModelManager>();
	buffer_master = std::make_unique<BufferMaster>();
	state_master = std::make_unique<StateMaster>();

	//camera
	active_camera = std::make_unique<FirstPersonCamera>();

	//world
	world = std::make_unique<World>(this);

	//elements
	sun = std::make_unique<Sun>();

	//entities
	player = std::make_unique<Player>(this);
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
			//load dev assets
			/*texture_manager->LoadBaseTextures();
			atlas->LoadBaseTextures();
			atlas->ReconstructTextureArray();
			model_manager->LoadBaseModels();
			buffer_master->RebuildDefaults();*/

			//setup scene elements
			//switch camera type
			// feed eye height to camera
			active_camera->SetDirection(1.0f, -1.0f, 0.0f);
			player->SetPosition(20.0f, 20.0f, 20.0f);
			active_camera->HookToEntity(player.get());
			//TODO: move to unit factory?
			physics_engine->RegisterMovementComponent(player.get());
			physics_engine->RegisterCollisionComponent(player.get());

			//generate the initial world
			world_engine->SetupStartingWorld();
			//world->SetupDevelopementWorld();
			
			//signal the start to the system engines
			physics_engine->Start();
			world_engine->BeginWorldGeneration();
			Supervisor::QueryService<EventHandler*>("event_handler")->Resume();

			break;
		}
		default:
			break;
	}
}

bool Scene::Initialize()
{
	physics_engine = Supervisor::QueryService<PhysicsEngine*>("physics_engine");
	world_engine = Supervisor::QueryService<WorldEngine*>("world_engine");

	if (!physics_engine || !world_engine)
		return false;

	bool _result = true;

	//initialize scene assets
	_result &= texture_manager->Initialize();
	_result &= atlas->Initialize();
	_result &= buffer_master->Initialize();
	_result &= state_master->Initialize();

	texture_manager->LoadBaseTextures();
	atlas->LoadBaseTextures();
	atlas->ReconstructTextureArray();
	model_manager->LoadBaseModels();
	buffer_master->RebuildDefaults();

	//_result &= model_manager->Initialize(); doesn't need initialization for now 

	//initialize the world

	//initialize world elements

	return _result;
}

void Scene::Update()
{
	player->Update();
	active_camera->Update();
	world->Update();
	world_engine->WorldLoadTick();
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
		_temp->SetDirection(active_camera->direction.x, active_camera->direction.y, active_camera->direction.z);
		break;
	}
	case CameraType::FIRST_PERSON:
	{
		_temp = std::make_unique<FirstPersonCamera>();
		_temp->SetProperties(active_camera->fov, active_camera->aspect_ratio, active_camera->near_plane, active_camera->far_plane);
		_temp->SetPosition(active_camera->position.x, active_camera->position.y, active_camera->position.z);
		_temp->SetDirection(active_camera->direction.x, active_camera->direction.y, active_camera->direction.z);
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

ID3D11Device* Scene::GetDevice()
{
	return presenter->GetDevice();
}

ID3D11DeviceContext* Scene::GetContext()
{
	return presenter->GetContext();
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
