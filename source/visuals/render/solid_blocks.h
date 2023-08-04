
#ifndef SOLID_BLOCKS_H
	#define SOLID_BLOCKS_H

	#include <vector>

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class DepthMap;
	class RenderTarget;
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

	private:
		friend class Overlay;

		void RenderSegments(bool renderAll = false);

		std::unique_ptr<VisionPerimeter> perimeter;
		std::vector<Segment*>
			near_segments,
			visible_segments;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>>
			per_object_buffer,
			sun_direction_buffer,
			bias_buffer,
			light_buffer;

		std::unique_ptr<VertexShader>
			vs_shadowmap_depth,
			vs_shadowmap_render;

		std::unique_ptr<PixelShader>
			ps_shadowmap_depth,
			ps_shadowmap_render;

		std::unique_ptr<DepthMap> depth_map;
		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> texture_overlay_buffer;
		std::unique_ptr<VertexShader> texture_overlay_vertex_shader;
		std::unique_ptr<PixelShader> texture_overlay_pixel_shader;
		std::unique_ptr<InputLayout> texture_overlay_input_layout;

		bool render_shadows;
		bool show_depth_map;
	};

#endif // !SOLID_BLOCKS_H
