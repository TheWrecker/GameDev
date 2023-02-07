
#include "service_manager.h"
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
	return nullptr;
}