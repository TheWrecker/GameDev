
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
	container[name] = std::move(std::unique_ptr<IService>(target));
}

ServiceManager::ServiceContainer& ServiceManager::Services()
{
	return container;
}

IService* ServiceManager::GetService(const std::string& name)
{
	auto _result = container.find(name);
	if (_result != container.end())
		return _result->second.get();
	return nullptr;
}