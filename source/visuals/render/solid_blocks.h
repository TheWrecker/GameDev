
#ifndef SOLID_BLOCKS_H
	#define SOLID_BLOCKS_H

	#include <vector>

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class Segment;
	class VisionPerimeter;
	class Presenter;

	class SolidBlockRender : public RenderBase
	{
	public:
		SolidBlockRender(Presenter* parent);
		~SolidBlockRender();

		bool Initialize() override;
		void Update() override;
		void Render() override;

		bool test_bool;

	private:
		friend class Overlay;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_object_buffer;
		std::unique_ptr<VisionPerimeter> perimeter;
		std::vector<Segment*> render_segments;
	};

#endif // !SOLID_BLOCKS_H
