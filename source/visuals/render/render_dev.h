
#ifndef RENDER_DEV_H
	#define RENDER_DEV_H

	#include <memory>

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class DepthMap;
	class RenderTarget;
	class TransformableEntity;
	class Presenter;

	class DevRender : public RenderBase
	{
	public:
		DevRender(Presenter* parent);
		~DevRender();

		bool Initialize() override;
		void Render() override;

	private:
		friend class Overlay;

		struct DevData
		{
			DirectX::XMFLOAT4
				ambient_color,
				light_color,
				specular_color;

			DirectX::XMFLOAT3 light_position;
			float light_radius;

			DirectX::XMFLOAT3 camera_position;
			float specular_power;

			DirectX::XMMATRIX projector_matrix;
		} dev_data;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> object_buffer;
		std::unique_ptr<ConstantBuffer<DevData>> dev_buffer;

		std::unique_ptr<VertexShader> vertex_shader2;
		std::unique_ptr<InputLayout> input_layout_depth;
		std::unique_ptr<InputLayout> input_layout_plane;
		std::unique_ptr<DepthMap> depth_map;
		std::unique_ptr<RenderTarget> render_target;
		std::unique_ptr<PixelShader> pixel_shader2;

		std::unique_ptr<TransformableEntity> object;
		std::unique_ptr<TransformableEntity> plane;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> texture_overlay_buffer;
		std::unique_ptr<VertexShader> texture_overlay_vertex_shader;
		std::unique_ptr<PixelShader> texture_overlay_pixel_shader;
		std::unique_ptr<InputLayout> texture_overlay_input_layout;

		bool use_render_target_as_depth_map;
	};

#endif // !RENDER_DEV_H
