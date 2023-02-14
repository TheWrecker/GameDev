
#pragma once

#include <d3d11.h>
#include <optional>

class TextureSampler
{
public:
	TextureSampler(ID3D11Device* device, ID3D11DeviceContext* context, std::optional<D3D11_SAMPLER_DESC> desc);
	~TextureSampler();

	void Build();
	void Bind(unsigned int slot);
	void Unbind();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11SamplerState* sampler_state;
	D3D11_SAMPLER_DESC desc;
	unsigned int current_slot;
};