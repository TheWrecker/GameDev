
#ifndef SUPERVISOR_H
	#define SUPERVISOR_H

	#include <d3d11.h>
	#include <memory>
	#include <string>

	#include "defs_platform.h"
	#include "service_manager.h"

	class Supervisor
	{
	public:
		Supervisor(InstanceHandle instance);
		~Supervisor();

		void PassControl();

		void SetDebugQuery(ID3D11Debug* target);
		ID3D11Debug* GetDebugQuery();
		ServiceManager* Services();

		template <typename type>
		type QueryService(const std::string& name);

	private:
		std::unique_ptr<ServiceManager> services;
		ID3D11Debug* d3d11_debug = nullptr;
	};

	template<typename type>
	inline type Supervisor::QueryService(const std::string& name)
	{
		return dynamic_cast<type>(services.get()->GetService(name));
	}

#endif // !SUPERVISOR_H
