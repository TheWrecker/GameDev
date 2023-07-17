
#ifndef RENDER_BASE_H
	#define RENDER_BASE_H

	#include <d3d11.h>
	#include <memory>

	class PixelShader;
	class VertexShader;
	class IndexBuffer;
	class InputLayout;
	class BufferMaster;
	class TextureManager;
	class ModelManager;
	class StateMaster;
	class BasicCamera;
	class Scene;
	class Aggregator;
	class Presenter;

	class RenderBase
	{
	public:
		RenderBase(Presenter* parent);
		virtual ~RenderBase();

		virtual bool Initialize();
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
		Aggregator* aggregator;
		Scene* scene;
		BasicCamera* camera;
		StateMaster* state_master;
		ModelManager* model_manager;
		TextureManager* texture_manager;
		BufferMaster* buffer_master;
	};

#endif // !RENDER_BASE_H
