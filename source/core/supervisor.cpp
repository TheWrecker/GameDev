
#include "../external/ImGui/imgui.h"

#include "service_manager.h"
#include "platform.h"
#include "sys_profiler.h"
#include "sys_ticker.h"
#include "../gameplay/game_time.h"
#include "../input/mouse.h"
#include "../input/keyboard.h"
#include "../visuals/presenter.h"
#include "../events/event_handler.h"
#include "../input/input_handler.h"
#include "../gameplay/physics_engine.h"

#include "supervisor.h"

Supervisor::Supervisor(InstanceHandle instance)
{
	services = std::make_unique<ServiceManager>();

	ImGui::CreateContext();

	IService* _platform = new Platform(instance);
	services->AdoptService("platform", _platform);
	IService* _ticker = new SystemTicker();
	services->AdoptService("ticker", _ticker);
	IService* _profiler = new SystemProfiler();
	services->AdoptService("profiler", _profiler);
	IService* _game_time = new GameTime(static_cast<SystemTicker*>(_ticker));
	services->AdoptService("game_time", _game_time);
	IService* _physics_engine = new PhysicsEngine(this);
	services->AdoptService("physics_engine", _physics_engine);
	IService* _mouse = new Mouse(static_cast<Platform*>(_platform)->GetWindowHandle());
	services->AdoptService("mouse", _mouse);
	IService* _keyboard = new Keyboard();
	services->AdoptService("keyboard", _keyboard);
	IService* _presenter = new Presenter(this);
	services->AdoptService("presenter", _presenter);
	IService* _event_handler = new EventHandler(this);
	services->AdoptService("event_handler", _event_handler);
	IService* _input_handler = new InputHandler(this);
	services->AdoptService("input_handler", _input_handler);

}

Supervisor::~Supervisor()
{
}

ServiceManager* Supervisor::Services()
{
	return services.get();
}

void Supervisor::PassControl()
{
	Platform* _platform = services->QueryService<Platform*>("platform");
	Presenter* _presenter = services->QueryService<Presenter*>("presenter");
	while (!_platform->ProcessPlatfromMessages())
	{
		//update components
		for (auto& service : services->Services())
		{
			service.second->Update();
		}

		//render
		_presenter->Draw();

	}
}

void Supervisor::SetDebugQuery(ID3D11Debug* target)
{
	d3d11_debug = target;
}

ID3D11Debug* Supervisor::GetDebugQuery()
{
	return d3d11_debug;
}
