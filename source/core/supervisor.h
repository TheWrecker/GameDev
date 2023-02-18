
#pragma once

#include <d3d11.h>
#include <memory>

#include "defs_platform.h"

class ServiceManager;

class Supervisor
{
public:
	Supervisor(InstanceHandle instance);
	~Supervisor();

	void PassControl();

	void SetDebugQuery(ID3D11Debug* target);
	ID3D11Debug* GetDebugQuery();
	ServiceManager* Services();

private:
	std::unique_ptr<ServiceManager> services;
	ID3D11Debug* d3d11_debug = nullptr;
};
