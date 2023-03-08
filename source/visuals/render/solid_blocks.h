
#pragma once

#include "../elements/buffer_constant.h"
#include "render_base.h"

class SolidBlockRender : public RenderBase
{
public:
	SolidBlockRender(Scene* scene);
	~SolidBlockRender();

	void Render() override;

private:
	friend class Overlay;

	std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_object_buffer;
};
