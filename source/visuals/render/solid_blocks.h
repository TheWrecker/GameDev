
#ifndef SOLID_BLOCKS_H
	#define SOLID_BLOCKS_H

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class Segment;
	class VisionPerimeter;

	class SolidBlockRender : public RenderBase
	{
	public:
		SolidBlockRender(Scene* scene);
		~SolidBlockRender();

		void Update() override;
		void Render() override;

	private:
		friend class Overlay;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_object_buffer;
		std::unique_ptr<VisionPerimeter> perimeter;
		std::vector<Segment*> render_segments;
	};

#endif // !SOLID_BLOCKS_H
