
#include "directxtk/DDSTextureLoader.h"
#include "directxtk/WICTextureLoader.h"

#include "defs_pipeline.h"
#include "util_funcs.h"
#include "../presenter.h"

#include "texture_atlas.h"

constexpr unsigned int ATLAS_RESERVE_COUNT = 64;
constexpr auto ATLAS_BASE_PATH = L"assets/textures/atlas/test_checkers.png";

TextureAtlas::TextureAtlas(Presenter* presenter)
    :presenter(presenter), texture_array(), base_desc(), shader_view(), textures(), descs(), views(), slice_count(0)
{
	device = presenter->GetDevice();
	context = presenter->GetContext();

	textures.reserve(ATLAS_RESERVE_COUNT);
	descs.reserve(ATLAS_RESERVE_COUNT);
	views.reserve(ATLAS_RESERVE_COUNT);
	LoadTexture(ATLAS_BASE_PATH, "test_checkers"); //test checkers = 0

	base_desc.Width = descs[0]->Width;
	base_desc.Height = descs[0]->Height;
	base_desc.MipLevels = descs[0]->MipLevels;
	base_desc.Usage = D3D11_USAGE_DEFAULT;
	base_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	base_desc.Format = descs[0]->Format;
	base_desc.SampleDesc.Count = 1;
	base_desc.SampleDesc.Quality = 0;
	base_desc.ArraySize = 0;

	//TODO: move to scene? dynamic lookup? list? config file?
	LoadTexture(L"assets/textures/atlas/dirt.png", "dirt"); //dirt = 1
	LoadTexture(L"assets/textures/atlas/grass.png", "grass"); //grass = 2

	ReconstructTextureArray();
}

TextureAtlas::~TextureAtlas()
{
	Unbind();
	DXRelease(texture_array);
	DXRelease(shader_view);

	for (auto& _item : textures)
		DXRelease(_item);

	for (auto _desc : descs)
		delete _desc;

	for (auto& _item : views)
		DXRelease(_item);
}

void TextureAtlas::ReconstructTextureArray()
{
	DXRelease(texture_array);
	DXRelease(shader_view);

	unsigned int _slices = static_cast<unsigned int>(textures.size());
	if (_slices == 0)
		return ;

	base_desc.ArraySize = _slices;
	for (size_t i = 0; i < _slices; i++)
	{
		if (descs[i]->MipLevels != descs[0]->MipLevels || descs[i]->Width != descs[0]->Width ||
			descs[i]->Height != descs[0]->Height ||	descs[i]->Format != descs[0]->Format)
		{
			assert(false);
			return;
		}
	}

	DXAssert(device->CreateTexture2D(&base_desc, NULL, &texture_array));

	unsigned int _mips = base_desc.MipLevels;
	unsigned int _current_slice = 0;
	unsigned int _current_mip = 0;
	unsigned int _current_subresource = 0;
	for (auto _texture : textures)
	{
		for (_current_mip = 0; _current_mip < _mips; _current_mip++)
		{
			_current_subresource = D3D11CalcSubresource(_current_mip, _current_slice, _mips);
			context->CopySubresourceRegion(texture_array, _current_subresource, 0, 0, 0, textures[_current_slice], _current_mip, 0);
		}
		_current_slice++;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC _shader_view_desc = {};
	_shader_view_desc.Format = base_desc.Format;
	_shader_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	_shader_view_desc.Texture2DArray.ArraySize = _slices;
	_shader_view_desc.Texture2DArray.FirstArraySlice = 0;
	_shader_view_desc.Texture2DArray.MipLevels = _mips;
	_shader_view_desc.Texture2DArray.MostDetailedMip = 0;
	DXAssert(device->CreateShaderResourceView(texture_array, &_shader_view_desc, &shader_view));
	slice_count = _slices;
}

void TextureAtlas::Clear()
{
	Unbind();

	for (auto _texture : textures)
		_texture->Release();

	for (auto _desc : descs)
		delete _desc;

	for (auto _view : views)
		_view->Release();

	DXRelease(shader_view);
	DXRelease(texture_array);

	textures.clear();
	descs.clear();
	views.clear();

	ReconstructTextureArray();
}

unsigned int TextureAtlas::LoadTexture(const std::wstring& file, const std::string& name)
{
	ID3D11Texture2D* _texture = nullptr;
	ID3D11ShaderResourceView* _view = nullptr;
	auto extension = file.substr(file.size() - 3, 3);
	if (extension == L"dds")
	{
		DXAssert(DirectX::CreateDDSTextureFromFile(device, file.c_str(), (ID3D11Resource**)&_texture, &_view));
	}
	else
	{
		DXAssert(DirectX::CreateWICTextureFromFile(device, file.c_str(), (ID3D11Resource**)&_texture, &_view));
	}
	textures.push_back(_texture);
	views.push_back(_view);
	D3D11_TEXTURE2D_DESC* _desc = new D3D11_TEXTURE2D_DESC;
	_texture->GetDesc(_desc);
	descs.push_back(_desc);
	unsigned int _index = static_cast<unsigned int>(textures.size()) - 1;
	names.insert(std::pair(name, _index));

	return _index;
}

unsigned int TextureAtlas::FindTextureIndex(const std::string& name)
{
	auto _result = names.find(name);
	if (_result == names.end())
		return 0; //test checkers
	return _result->second;
}

void TextureAtlas::Bind()
{
	context->PSSetShaderResources(static_cast<unsigned int>(DefaultTextures::TEXTURE_ATLAS), 1, &shader_view);
}

void TextureAtlas::Unbind()
{
	ID3D11ShaderResourceView* _null_view = {nullptr};
	context->PSSetShaderResources(static_cast<unsigned int>(DefaultTextures::TEXTURE_ATLAS), 1, &_null_view);
}

ID3D11Texture2D* TextureAtlas::GetTexture(size_t index)
{
    return textures[index];
}

ID3D11ShaderResourceView* TextureAtlas::GetShaderView(size_t index)
{
    return views[index];
}

ID3D11Texture2D* TextureAtlas::GetTextureArray()
{
	return texture_array;
}

ID3D11ShaderResourceView* TextureAtlas::GetArrayShaderView()
{
	return shader_view;
}

std::size_t TextureAtlas::GetTextureCount()
{
	return textures.size();
}

std::size_t TextureAtlas::GetArraySliceCount()
{
	return slice_count;
}
