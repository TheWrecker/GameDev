
#include "../external/ImGui/imgui.h"

#include "service_manager.h"
#include "platform.h"
#include "sys_profiler.h"
#include "sys_ticker.h"
#include "executor.h"
#include "../input/mouse.h"
#include "../input/keyboard.h"
#include "../visuals/presenter.h"
#include "../scene/scene.h"
#include "../gameplay/physics_engine.h"
#include "../gameplay/world_engine.h"
#include "../events/event_handler.h"
#include "../input/input_handler.h"
#include "../processors/processor_solid_block.h"
#include "../processors/processor_biome.h"

#include "supervisor.h"

Supervisor* Supervisor::last_instance = nullptr;

Supervisor::Supervisor(InstanceHandle instance)
{
	services = std::make_unique<ServiceManager>();

	ImGui::CreateContext();

	//TODO: change/improve service order in the vector?

	//create all services and add them to service manager
	IService* _platform = new Platform(instance);
	services->AdoptService("platform", _platform);

	IService* _ticker = new SystemTicker();
	services->AdoptService("ticker", _ticker);

	IService* _profiler = new SystemProfiler();
	services->AdoptService("profiler", _profiler);

	IService* _executor = new Executor();
	services->AdoptService("executor", _executor);

	IService* _mouse = new Mouse();
	services->AdoptService("mouse", _mouse);

	IService* _keyboard = new Keyboard();
	services->AdoptService("keyboard", _keyboard);

	IService* _presenter = new Presenter(this);
	services->AdoptService("presenter", _presenter);

	IService* _scene = new Scene(static_cast<Presenter*>(_presenter));
	services->AdoptService("scene", _scene);

	IService* _physics_engine = new PhysicsEngine();
	services->AdoptService("physics_engine", _physics_engine);

	IService* _world_engine = new WorldEngine(3); //dev seed
	services->AdoptService("world_engine", _world_engine);

	IService* _event_handler = new EventHandler();
	services->AdoptService("event_handler", _event_handler);

	IService* _input_handler = new InputHandler();
	services->AdoptService("input_handler", _input_handler);

	last_instance = this;
}

Supervisor::~Supervisor()
{
}

ServiceManager* Supervisor::Services()
{
	return services.get();
}

bool Supervisor::InitializeAllSystems()
{
	//TODO: add perstep checks and retries
	bool _result = true;

	//setup and start the windows infrastructure
	QueryService<Platform*>("platform")->SetWindowsParameters();
	_result &= QueryService<Platform*>("platform")->Initialize();

	//initialize the system profiler
	_result &= QueryService<SystemProfiler*>("profiler")->Initialize();

	//initialize the executor
	_result &= QueryService<Executor*>("executor")->Initialize();

	//initialize mouse input
	_result &= QueryService<Mouse*>("mouse")->Initialize();

	//initialize the presenter and its components
	_result &= QueryService<Presenter*>("presenter")->Initialize();

	//initialize the scene and its components
	_result &= QueryService<Scene*>("scene")->Initialize();

	//initialize the physics engine
	_result &= QueryService<PhysicsEngine*>("physics_engine")->Initialize();

	//initialize the world engine
	_result &= QueryService<WorldEngine*>("world_engine")->Initialize();

	//initialize the input handler
	_result &= QueryService<InputHandler*>("input_handler")->Initialize();

	//initialize processors
	_result &= SolidBlockProcessor::Setup();
	_result &= BiomeProcessor::Setup();

	return _result;
}

void Supervisor::PassControl()
{
	//switch scene mode to dev mode and load assets
	auto _scene = QueryService<Scene*>("scene");
	_scene->SwitchMode(SceneMode::DEVELOPEMENT);

	//load the world
	QueryService<WorldEngine*>("world_engine")->SetupStartingWorld();

	//start the executor
	QueryService<Executor*>("executor")->Resume();

	//start the physics engine
	QueryService<PhysicsEngine*>("physics_engine")->Start();

	//start the input handler
	QueryService<InputHandler*>("input_handler")->Resume();

	//start dynamic world generation
	QueryService<WorldEngine*>("world_engine")->BeginWorldGeneration();

	//begin the main engine loop
	auto _platform = QueryService<Platform*>("platform");
	auto _presenter = QueryService<Presenter*>("presenter");
	while (!_platform->ProcessPlatformMessages())
	{
		//update all services
		for (auto& service : services->Services())
		{
			service.second->Update();
		}

		//render
		_presenter->Draw();

	}
}
