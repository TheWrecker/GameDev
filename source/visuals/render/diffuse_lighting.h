
#pragma once

#include "../elements/buffer_vertex.h"
#include "../elements/buffer_constant.h"
#include "render_base.h"

class BasicEntity;

class DiffuseLighting : public RenderBase
{
public:
	DiffuseLighting(Scene* scene);
	~DiffuseLighting();

	void Render() override;

private:
	friend class Overlay;

	struct LightData
	{
		DirectX::XMFLOAT4 light_direction;
		DirectX::XMFLOAT4 light_color;
		DirectX::XMFLOAT4 ambient_color;
	};

	struct PerObjectBuffer
	{
		DirectX::XMMATRIX world;
	};

	LightData light_data;

	std::unique_ptr<ConstantBuffer<PerObjectBuffer>> object_buffer;
	std::unique_ptr<ConstantBuffer<LightData>> light_buffer;

	//TODO: add light entities
	std::unique_ptr<BasicEntity> light;
	std::unique_ptr<BasicEntity> object;
};
