
#pragma once

#include <memory>

#include "platfrom_defs.h"
#include "service_manager.h"

class Supervisor
{
public:
	Supervisor(InstanceHandle instance);
	~Supervisor();

	void PassControl();

private:

	std::unique_ptr<ServiceManager> services;

};
