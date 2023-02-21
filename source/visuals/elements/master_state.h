
#pragma once

#include <d3d11.h>
#include <vector>

class TextureSampler;
class Presenter;

enum class DefaultSampler
{
	UNDEFINED = 0,
	POINT = 13,
	BILINEAR = 14,
	END_PADDING = 15 //D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT (16)
};

class StateMaster
{
public:
	StateMaster(Presenter* parent);
	~StateMaster();

	void BindDefaultTextureSampler(DefaultSampler what);
	void UnbindDefaultTextureSampler(DefaultSampler what);
	unsigned int GetSamplerSlot(DefaultSampler target);

private:
	Presenter* presenter;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	std::vector<TextureSampler*> texture_samplers;
};