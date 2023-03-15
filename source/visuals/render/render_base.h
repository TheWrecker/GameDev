
#ifndef RENDER_BASE_H
	#define RENDER_BASE_H

	#include <d3d11.h>
	#include <memory>

	#include "defs_pipeline.h"
	#include "../elements/shader_pixel.h"
	#include "../elements/shader_vertex.h"
	#include "../entities/camera_basic.h"
	#include "../elements/input_layout.h"
	#include "../elements/buffer_index.h"
	#include "../elements/buffer_vertex.h"
	#include "../elements/buffer_constant.h"
	#include "../elements/manager_model.h"
	#include "../elements/manager_texture.h"
	#include "../elements/master_state.h"
	#include "../elements/master_buffer.h"

	class Scene;

	class RenderBase
	{
	public:
		RenderBase(Scene* scene);
		virtual ~RenderBase();

		virtual void Render() = 0;
		virtual void Update();

	protected:
		std::unique_ptr<InputLayout> input_layout;
		std::unique_ptr<IndexBuffer> index_buffer;
		std::unique_ptr<VertexShader> vertex_shader;
		std::unique_ptr<PixelShader> pixel_shader;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		Presenter* presenter;
		Scene* scene;
		BasicCamera* camera;
		StateMaster* state_master;
		ModelManager* model_manager;
		TextureManager* texture_manager;
		BufferMaster* buffer_master;
	};

#endif // !RENDER_BASE_H
