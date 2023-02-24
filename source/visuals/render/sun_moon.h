
#pragma once

#include "render_base.h"

class BasicCamera;
class Sun;

class SunMoon : public RenderBase
{
public:
	SunMoon(Scene* scene);
	~SunMoon();

	void Render() override;

private:
	struct PerFrameBuffer
	{
		DirectX::XMMATRIX world;
	};

	BasicCamera* camera;
	Sun* sun;
	std::unique_ptr<ConstantBuffer<PerFrameBuffer>> per_frame_buffer;
};