
#pragma once

#include <d3d11.h>
#include <unordered_map>

class TextureSampler;

enum class DefaultSampler
{
	POINT,
	BILINEAR
};

class SamplerManager
{
public:
	SamplerManager(ID3D11Device* device, ID3D11DeviceContext* context);
	~SamplerManager();

	void BindTextureSampler(DefaultSampler what, unsigned int slot);
	void UnbindTextureSampler(DefaultSampler what);

	static void BindDefaultTextureSampler(DefaultSampler what, unsigned int slot);
	static void UnbindDefaultTextureSampler(DefaultSampler what);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	std::unordered_map<DefaultSampler, TextureSampler*> texture_samplers;
};