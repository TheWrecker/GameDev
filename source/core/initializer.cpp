
#include "initializer.h"

Initializer::Initializer(InstanceHandle instance)
{
	platfrom = std::make_unique<Platform>(instance);

}
