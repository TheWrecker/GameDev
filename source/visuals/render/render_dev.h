
#ifndef RENDER_DEV_H
	#define RENDER_DEV_H

	#include <memory>

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class TransformableEntity;
	class Presenter;

	class DevRender : public RenderBase
	{
	public:
		DevRender(Presenter* parent);
		~DevRender();

		void Render() override;

	private:
		friend class Overlay;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> object_buffer;
		std::unique_ptr<TransformableEntity> object;
	};

#endif // !RENDER_DEV_H
