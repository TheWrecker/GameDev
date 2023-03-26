
#include <cassert>

#include "service_manager.h"

ServiceManager::ServiceManager()
	:container()
{
}

ServiceManager::~ServiceManager()
{
}

void ServiceManager::AdoptService(std::string name, IService* target)
{
	if (!target)
	{
		//atmepting to push nullptr as a service
		assert(false);
		return;
	}
	container.push_back(ServicePair(name, std::move(std::unique_ptr<IService>(target))));
}

ServiceManager::ServiceContainer& ServiceManager::Services()
{
	return container;
}

IService* ServiceManager::GetService(const std::string& name)
{
	for (auto& service : container)
	{
		if (service.first == name)
			return service.second.get();
	}

	//no such service exists
	assert(false);
	return nullptr;
}
