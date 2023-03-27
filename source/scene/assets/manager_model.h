
#ifndef MANAGER_MODEL_H
	#define MANAGER_MODEL_H

	#include "../visuals/elements/model.h"
	#include "interface_manager.h"

	class Model;

	class ModelManager : public IManager<Model>
	{
	public:
		ModelManager();
		~ModelManager();

		void Load(const std::wstring& file, bool flipUV = false, const std::string& name = "");
		void LoadBaseModels();
		//TODO: load models from a list/file
	};

#endif // !MANAGER_MODEL_H
