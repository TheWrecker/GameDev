
#pragma once

#include <memory>
#include <Windows.h>

#include "platfrom_defs.h"

#include "platform.h"

class Initializer
{

public:

	Initializer(InstanceHandle instance);
	~Initializer();

private:

	std::unique_ptr<Platform> platfrom;


};