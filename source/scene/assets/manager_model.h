
#ifndef MANAGER_MODEL_H
	#define MANAGER_MODEL_H

	#include "model.h"
	#include "interface_manager.h"

	class ModelManager : public IManager<Model>
	{
	public:
		ModelManager();
		~ModelManager();

		void Load(const std::wstring& file, bool flipUV = false, const std::string& name = "");
	};

#endif // !MANAGER_MODEL_H
