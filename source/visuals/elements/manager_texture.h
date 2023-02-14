
#pragma once

#include "interface_manager.h"

class Texture;
class Presenter;

class TextureManager : public IManager<Texture>
{
public:
	TextureManager(Presenter* presenter);
	~TextureManager();

	void Load(const std::wstring& file, const std::string& name = "");

private:
	Presenter* presenter;
};
