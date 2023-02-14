
#include "sampler_texture.h"

#include "sampler_manager.h"

SamplerManager* first_instance = nullptr;

SamplerManager::SamplerManager(ID3D11Device* device, ID3D11DeviceContext* context)
	:device(device), context(context), texture_samplers()
{
	D3D11_SAMPLER_DESC desc = {};
	//POINT
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	auto sampler1 = new TextureSampler(device, context, desc);
	texture_samplers.insert(std::pair(DefaultSampler::POINT, sampler1));

	//BILINEAR
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	auto sampler2 = new TextureSampler(device, context, desc);
	texture_samplers.insert(std::pair(DefaultSampler::BILINEAR, sampler2));

	if (!first_instance)
		first_instance = this;
}

SamplerManager::~SamplerManager()
{
	for (auto& item : texture_samplers)
	{
		item.second->Unbind();
		delete item.second;
	}
}

void SamplerManager::BindTextureSampler(DefaultSampler what, unsigned int slot)
{
	auto sampler = texture_samplers.find(what);
	if (sampler != texture_samplers.end())
		sampler->second->Bind(slot);
}

void SamplerManager::UnbindTextureSampler(DefaultSampler what)
{
	auto sampler = texture_samplers.find(what);
	if (sampler != texture_samplers.end())
		sampler->second->Unbind();
}

void SamplerManager::BindDefaultTextureSampler(DefaultSampler what, unsigned int slot)
{
	first_instance->BindTextureSampler(what, slot);
}

void SamplerManager::UnbindDefaultTextureSampler(DefaultSampler what)
{
	first_instance->UnbindTextureSampler(what);
}
