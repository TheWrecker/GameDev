
#pragma once

#include <DirectXMath.h>

#include "render_base.h"

class SkyRender : public RenderBase
{
public:
	SkyRender(Scene* scene);
	~SkyRender();


private:
	struct SkyVertex
	{
		DirectX::XMFLOAT4 position;
	};

	struct 
};