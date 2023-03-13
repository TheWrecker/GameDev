
#ifndef SUN_H
	#define SUN_H

	#include "entity_transformable.h"

	class Sun : public TransformableEntity
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

#endif // !SUN_H
