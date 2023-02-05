
#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "interface_service.h"

class ServiceManager
{
public:
	ServiceManager();
	~ServiceManager();

	void AdoptService(std::string name, IService* target);
	template <typename type>
	type QueryService(const std::string& name);

private:
	IService* GetService(const std::string& name);

	std::unordered_map<std::string, std::unique_ptr<IService>> container;
};

template<typename type>
inline type ServiceManager::QueryService(const std::string& name)
{
	return dynamic_cast<type>(GetService(name));
}
