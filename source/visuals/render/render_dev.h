
#pragma once

#include <d3d11.h>
#include <memory>

#include "../entities/entity_basic.h"
#include "render_base.h"

class DevRender : public RenderBase
{
public:
	DevRender(Scene* scene);
	~DevRender();

	void Render() override;

private:
	struct PerObjectBuffer
	{
		DirectX::XMMATRIX world;
	};

	std::unique_ptr<BasicEntity> test_object;
	std::unique_ptr<ConstantBuffer<PerObjectBuffer>> per_object_buffer;
};
