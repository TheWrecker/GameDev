
#pragma once

#include "interface_manager.h"

class Model;

class ModelManager : public IManager<Model>
{
public:
	ModelManager();
	~ModelManager();

	void Load(const std::wstring& file, bool flipUV = false, const std::string& name = "");
};
