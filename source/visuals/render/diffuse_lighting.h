
#pragma once

#include "../elements/buffer_vertex.h"
#include "../elements/buffer_constant.h"
#include "render_base.h"

class TransformableEntity;

class DiffuseLighting : public RenderBase
{
public:
	DiffuseLighting(Scene* scene);
	~DiffuseLighting();

	void Render() override;

private:
	friend class Overlay;

	std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> object_buffer;

	//TODO: add light entities
	std::unique_ptr<TransformableEntity> object;
};
