
#pragma once

#include <d3d11.h>
#include <string>
#include <vector>
#include <unordered_map>

class Presenter;

class TextureAtlas
{
public:
	TextureAtlas(Presenter* presenter);
	~TextureAtlas();

	void ReconstructTextureArray();
	void Clear();

	unsigned int LoadTexture(const std::wstring& file, const std::string& name);
	unsigned int FindTextureIndex(const std::string& name);
	void Bind();

	ID3D11Texture2D* GetTexture(size_t index);
	ID3D11ShaderResourceView* GetShaderView(size_t index);
	ID3D11Texture2D* GetTextureArray();
	ID3D11ShaderResourceView* GetArrayShaderView();

	std::size_t GetTextureCount();
	std::size_t GetArraySliceCount();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Presenter* presenter;
	ID3D11Texture2D* texture_array;
	D3D11_TEXTURE2D_DESC base_desc;
	ID3D11ShaderResourceView* shader_view;
	std::vector<ID3D11Texture2D*> textures;
	std::vector<D3D11_TEXTURE2D_DESC*> descs;
	std::vector<ID3D11ShaderResourceView*> views;
	std::size_t slice_count;
	std::unordered_map<std::string, unsigned int> names;
};