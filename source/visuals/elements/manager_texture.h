
#pragma once

#include <d3d11.h>

#include "defs_pipeline.h"
#include "texture.h"
#include "interface_manager.h"

class Presenter;

class TextureManager : public IManager<Texture>
{
public:
	TextureManager(Presenter* presenter);
	~TextureManager();

	void Load(const std::wstring& file, const std::string& name = "");
	void BindDefaultTextures();
	ID3D11ShaderResourceView* GetShaderView(const std::string& name);
	unsigned int GetDefaultTextureIndex(DefaultTextures target);

private:
	Presenter* presenter;
};
