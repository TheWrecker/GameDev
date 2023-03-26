
#ifndef SUPERVISOR_H
	#define SUPERVISOR_H

	#include <cassert>
	#include <memory>
	#include <string>

	#include "defs_platform.h"

	class ServiceManager;

	class Supervisor
	{
	public:
		Supervisor(InstanceHandle instance);
		~Supervisor();

		bool InitializeAllSystems();
		void PassControl();

		ServiceManager* Services();

		template <typename type>
		type GetService(const std::string& name);

		template <typename type>
		static type QueryService(const std::string& name);

	private:
		std::unique_ptr<ServiceManager> services;

		static Supervisor* last_instance;
	};

	template<typename type>
	inline type Supervisor::GetService(const std::string& name)
	{
		return services->QueryService<type>(name);
	}

	template<typename type>
	inline static type Supervisor::QueryService(const std::string& name)
	{
		assert(last_instance);
		return last_instance->services->QueryService<type>(name);
	}

#endif // !SUPERVISOR_H
