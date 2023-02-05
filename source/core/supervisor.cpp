
#include "platform.h"
#include "sys_ticker.h"
#include "sys_profiler.h"

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
}

Supervisor::~Supervisor()
{
}

void Supervisor::PassControl()
{
	int i = 0;
	Platform* _platform = services->QueryService<Platform*>("platform");
	while (!_platform->ProcessPlatfromMessages())
	{
		i++;

	}
}
