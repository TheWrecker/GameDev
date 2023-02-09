
#pragma once

#include <vector>
#include <memory>
#include <string>

#include "interface_service.h"

class ServiceManager
{
public:
	typedef std::pair<std::string, std::unique_ptr<IService>> ServicePair;
	typedef std::vector<ServicePair> ServiceContainer;

	ServiceManager();
	~ServiceManager();

	void AdoptService(std::string name, IService* target);
	template <typename type>
	type QueryService(const std::string& name);
	ServiceContainer& Services();

private:
	friend class Supervisor;
	friend class Presenter;

	IService* GetService(const std::string& name);

	ServiceContainer container;
};

template<typename type>
inline type ServiceManager::QueryService(const std::string& name)
{
	return dynamic_cast<type>(GetService(name));
}
