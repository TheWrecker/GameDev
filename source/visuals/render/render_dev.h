
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
	std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_object_buffer;
};
