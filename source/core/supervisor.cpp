
#include "platform.h"
#include "service_manager.h"
#include "sys_profiler.h"
#include "sys_ticker.h"
#include "../visuals/presenter.h"

#include "supervisor.h"

Supervisor::Supervisor(InstanceHandle instance)
{
	services = std::make_unique<ServiceManager>();
	IService* _platform = new Platform(instance);
	services->AdoptService("platform", _platform);
	IService* _ticker = new SystemTicker();
	services->AdoptService("ticker", _ticker);
	IService* _profiler = new SystemProfiler();
	services->AdoptService("profiler", _profiler);
	IService* _presenter = new Presenter(this);
	services->AdoptService("presenter", _presenter);
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
	int i = 0;
	Platform* _platform = services->QueryService<Platform*>("platform");
	Presenter* _presenter = services->QueryService<Presenter*>("presenter");
	while (!_platform->ProcessPlatfromMessages())
	{
		//update components
		_presenter->Update();

		//render
		_presenter->Draw();

	}
}
