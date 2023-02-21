
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

	struct PerObjectBuffer
	{
		DirectX::XMMATRIX world;
	};

	std::unique_ptr<ConstantBuffer<PerObjectBuffer>> object_buffer;

	//TODO: add light entities
	std::unique_ptr<BasicEntity> object;
};
