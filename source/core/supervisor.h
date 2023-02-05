
#pragma once

#include <memory>

#include "platfrom_defs.h"

class ServiceManager;

class Supervisor
{
public:
	Supervisor(InstanceHandle instance);
	~Supervisor();

	ServiceManager* Services();
	void PassControl();

private:
	std::unique_ptr<ServiceManager> services;

};
