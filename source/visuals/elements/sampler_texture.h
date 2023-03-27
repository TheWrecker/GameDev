
#ifndef SAMPLER_TEXTURE_H
	#define SAMPLER_TEXTURE_H

	#include <d3d11.h>

	class TextureSampler
	{
	public:
		TextureSampler(ID3D11Device* device, ID3D11DeviceContext* context, D3D11_SAMPLER_DESC& desc);
		~TextureSampler();

		void Build();
		void Bind(unsigned int slot);
		void Unbind();

		D3D11_SAMPLER_DESC desc;

	private:
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		ID3D11SamplerState* sampler_state;
		unsigned int current_slot;
	};

#endif // !SAMPLER_TEXTURE_H
