
#ifndef MANAGER_TEXTURE_H
	#define MANAGER_TEXTURE_H

	#include <d3d11.h>

	#include "defs_pipeline.h"
	#include "../visuals/elements/texture.h"
	#include "interface_manager.h"

	class Texture;

	class TextureManager : public IManager<Texture>
	{
	public:
		TextureManager();
		~TextureManager();

		bool Initialize();
		void LoadBaseTextures();
		void BindDefaultTextures();

		void Load(const std::wstring& file, const std::string& name = "");

		ID3D11ShaderResourceView* GetShaderView(const std::string& name);
		unsigned int GetDefaultTextureIndex(DefaultTextures target);
		//TODO: Load from file/list?

	private:
		ID3D11Device* device;
		ID3D11DeviceContext* context;
	};

#endif // !MANAGER_TEXTURE_H
