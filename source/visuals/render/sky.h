
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
	struct PerFrameBuffer
	{
		DirectX::XMMATRIX world;
	};

	BasicCamera* camera;
	std::unique_ptr<ConstantBuffer<PerFrameBuffer>> per_frame_buffer;
};