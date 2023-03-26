
#ifndef IMANAGER_H
	#define IMANAGER_H

	#include <string>
	#include <unordered_map>

	template <typename type>
	class IManager
	{
	public:
		IManager()
			:container()
		{
		}
		virtual ~IManager()
		{
			for (auto& item : container)
			{
				delete item.second;
			}
		}

		virtual void Load(const std::wstring& file)
		{
		}

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

	protected:
		std::unordered_map<std::string, type*> container;
	};

#endif // !IMANAGER_H
