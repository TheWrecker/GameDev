
#ifndef RENDER_DEV_H
	#define RENDER_DEV_H

	#include <d3d11.h>
	#include <memory>

	#include "../entities/entity_basic.h"
	#include "render_base.h"

	class TransformableEntity;

	class DevRender : public RenderBase
	{
	public:
		DevRender(Scene* scene);
		~DevRender();

		void Render() override;

	private:
		friend class Overlay;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> object_buffer;
		std::unique_ptr<TransformableEntity> object;
	};

#endif // !RENDER_DEV_H
