
#pragma once

#include "render_base.h"

class BasicCamera;

class SkyRender : public RenderBase
{
public:
	SkyRender(Scene* scene);
	~SkyRender();

	void Render() override;

private:

	DirectX::XMMATRIX scale_matrix;

	BasicCamera* camera;
	std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_frame_buffer;
};