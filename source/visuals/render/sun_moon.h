
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

	BasicCamera* camera;
	Sun* sun;
	std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_frame_buffer;
};