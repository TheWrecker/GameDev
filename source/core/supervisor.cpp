
#include "../external/ImGui/imgui.h"

#include "service_manager.h"
#include "platform.h"
#include "sys_profiler.h"
#include "sys_ticker.h"
#include "executor.h"
#include "garbage_collector.h"
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
	:executor_available(false)
{
	services = std::make_unique<ServiceManager>();

	ImGui::CreateContext();

	//TODO: change/improve service order in the vector?

	//setup and start the windows infrastructure
	auto _platform = new Platform(instance);
	_platform->SetWindowsParameters();
	_platform->Initialize();

	//create all services and add them to service manager
	services->AdoptService("platform", _platform);

	IService* _ticker = new SystemTicker();
	services->AdoptService("ticker", _ticker);

	IService* _profiler = new SystemProfiler();
	services->AdoptService("profiler", _profiler);

	IService* _executor = new Executor(this);
	services->AdoptService("executor", _executor);

	IService* _garbage_collector = new GarbageCollector();
	services->AdoptService("garbage_collector", _garbage_collector);

	IService* _mouse = new Mouse();
	services->AdoptService("mouse", _mouse);

	IService* _keyboard = new Keyboard();
	services->AdoptService("keyboard", _keyboard);

	RECT _rect = {};
	GetClientRect(_platform->GetWindowHandle(), &_rect);
	IService* _presenter = new Presenter(this, _rect.right - _rect.left, _rect.bottom - _rect.top);
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

bool Supervisor::IsExecutorAvailable()
{
	return executor_available;
}

Supervisor* Supervisor::GetLastInstance()
{
	return last_instance;
}

void Supervisor::ExecutorDestroyed()
{
	executor_available = false;
}

bool Supervisor::InitializeAllSystems()
{
	//TODO: add perstep checks and retries
	bool _result = true;

	//initialize the system profiler
	_result &= QueryService<SystemProfiler*>("profiler")->Initialize();

	//initialize the executor
	_result &= QueryService<Executor*>("executor")->Initialize();
	executor_available = true;

	//initialize mouse input
	_result &= QueryService<Mouse*>("mouse")->Initialize();

	//initialize the scene and its components
	_result &= QueryService<Scene*>("scene")->Initialize();

	//initialize the presenter and its components
	_result &= QueryService<Presenter*>("presenter")->Initialize();

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

	//start the executor
	auto _executor = QueryService<Executor*>("executor");
	_executor->Resume();

	//start the input handler
	QueryService<InputHandler*>("input_handler")->Resume();

	//begin the main engine loop
	auto _platform = QueryService<Platform*>("platform");
	auto _presenter = QueryService<Presenter*>("presenter");

	//test
	auto _draw_func = [&]()
	{
		_presenter->Draw();
		_presenter->Present();
	};
	//auto _ret_f = _executor->StartExecution(_draw_func);

	while (!_platform->ProcessPlatformMessages())
	{
		//update all services
		for (auto& service : services->Services())
		{
			service.second->Update();
		}

		//render
		_presenter->Draw();
		_presenter->Present();
	}

	//_executor->StopExecution(_ret_f);
}
