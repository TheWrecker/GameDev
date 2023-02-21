
#include "sampler_texture.h"
#include "../presenter.h"

#include "master_state.h"

StateMaster::StateMaster(Presenter* parent)
	:presenter(parent), texture_samplers()
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
	texture_samplers.resize(GetSamplerSlot(DefaultSampler::END_PADDING));

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
	texture_samplers[GetSamplerSlot(DefaultSampler::POINT)] = sampler1;

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
	texture_samplers[GetSamplerSlot(DefaultSampler::BILINEAR)] = sampler2;
}

StateMaster::~StateMaster()
{
	for (auto item : texture_samplers)
		delete item;
}

void StateMaster::BindDefaultTextureSampler(DefaultSampler what)
{
	auto _index = GetSamplerSlot(what);
	texture_samplers[_index]->Bind(_index);
}

void StateMaster::UnbindDefaultTextureSampler(DefaultSampler what)
{
	auto _index = GetSamplerSlot(what);
	texture_samplers[_index]->Unbind();
}

unsigned int StateMaster::GetSamplerSlot(DefaultSampler target)
{
	return static_cast<unsigned int>(target);
}
