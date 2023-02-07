
#include "../external/ImGui/imgui.h"

#include "../input/mouse.h"
#include "../input/keyboard.h"
#include "platform.h"
#include "service_manager.h"
#include "sys_profiler.h"
#include "sys_ticker.h"
#include "../visuals/presenter.h"

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
	IService* _presenter = new Presenter(this);
	services->AdoptService("presenter", _presenter);
	IService* _mouse = new Mouse(static_cast<Platform*>(_platform)->GetWindowHandle());
	services->AdoptService("mouse", _mouse);
	IService* _keyboard = new Keyboard();
	services->AdoptService("keyboard", _keyboard);
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
