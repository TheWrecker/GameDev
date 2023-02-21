
#pragma once

#include "entity_basic.h"

class Sun : public BasicEntity
{
public:
	Sun();
	~Sun();

	DirectX::XMMATRIX GetLightInfo();

private:
	friend class Overlay;

	struct LightInfo
	{
		DirectX::XMFLOAT4
			direction,
			color,
			ambient,
			UNUSED;
	};
	LightInfo light_info;
};