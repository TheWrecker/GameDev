
#include "util_funcs.h"

#include "sampler_texture.h"

TextureSampler::TextureSampler(ID3D11Device* device, ID3D11DeviceContext* context, std::optional<D3D11_SAMPLER_DESC> info)
	:device(device), context(context), current_slot(0), sampler_state()
{
	if (info.has_value())
	{
		desc = info.value();
	}
	else
	{
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
	}
	device->CreateSamplerState(&desc, &sampler_state);
}

TextureSampler::~TextureSampler()
{
	Unbind();
	DXRelease(sampler_state);
}

void TextureSampler::Build()
{
	DXRelease(sampler_state);
	device->CreateSamplerState(&desc, &sampler_state);
}

void TextureSampler::Bind(unsigned int slot)
{
	context->PSSetSamplers(slot, 1, &sampler_state);
	current_slot = slot;
}

void TextureSampler::Unbind()
{
	ID3D11SamplerState* _null_sampler = { nullptr };
	context->PSSetSamplers(current_slot, 1, &_null_sampler);
}
