
#pragma once

#include <string>
#include <unordered_map>

template <typename type>
class IManager
{
public:
	IManager()
		:container()
	{
		if (!primary_instance)
			primary_instance = this;
	}
	virtual ~IManager()
	{
		for (auto& item : container)
		{
			delete item.second;
		}
	}

	virtual void Load() = 0;

	virtual void Unload(const std::string& name)
	{
		if (container.contains(name))
		{
			auto _target = container.find(name);
			delete _target->second;
			container.erase(name);
		}
	}

	type* Get(const std::string& name)
	{
		if (container.contains(name))
		{
			auto _target = container.find(name);
			return _target->second;
		}
		return nullptr;
	}

	static type* GetPrimaryManager()
	{
		return primary_instance;
	}

protected:
	std::unordered_map<std::string, type*> container;

	static IManager* primary_instance;
};
