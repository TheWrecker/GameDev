
#include "util_funcs.h"

#include "sampler_texture.h"

TextureSampler::TextureSampler(ID3D11Device* device, ID3D11DeviceContext* context, D3D11_SAMPLER_DESC& info)
	:device(device), context(context), current_slot(0), sampler_state()
{
	desc = info;
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
