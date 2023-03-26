
#ifndef SERVICE_MANAGER_H
	#define SERVICE_MANAGER_H

	#include <vector>
	#include <memory>
	#include <string>

	class IService;

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
		IService* GetService(const std::string& name);

		ServiceContainer container;
	};

	template<typename type>
	inline type ServiceManager::QueryService(const std::string& name)
	{
		#ifdef _DEBUG
			return dynamic_cast<type>(GetService(name));
		#else
			return static_cast<type>(GetService(name));
		#endif
	}

#endif // !SERVICE_MANAGER_H
