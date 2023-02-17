
#pragma once

#include <d3d11.h>
#include <unordered_map>

class TextureSampler;
class Presenter;

enum class DefaultSampler
{
	POINT,
	BILINEAR
};

class StateMaster
{
public:
	StateMaster(Presenter* parent);
	~StateMaster();

	void BindTextureSampler(DefaultSampler what, unsigned int slot);
	void UnbindTextureSampler(DefaultSampler what);

private:
	Presenter* presenter;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	std::unordered_map<DefaultSampler, TextureSampler*> texture_samplers;
};