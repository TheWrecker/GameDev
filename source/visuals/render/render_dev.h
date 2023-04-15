
#ifndef RENDER_DEV_H
	#define RENDER_DEV_H

	#include <memory>

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class DepthPass;
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
		std::unique_ptr<DepthPass> depth_pass;

		std::unique_ptr<TransformableEntity> object;
		std::unique_ptr<TransformableEntity> plane;
	};

#endif // !RENDER_DEV_H
