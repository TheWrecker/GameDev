
#pragma once

#include <cstddef>

class SystemProfilerInterface
{
public:
	virtual void QueryRAMCapacity(std::size_t& physicalTotal, std::size_t& virtualTotal) = 0;
	virtual void QueryCPUStats(std::size_t& gameConsumption, std::size_t& totalConsumption) = 0;
	virtual void QueryRAMStats(std::size_t& physicalUsed, std::size_t& virtualUsed) = 0;
};