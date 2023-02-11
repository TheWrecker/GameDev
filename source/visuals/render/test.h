
#pragma once

//TODO: change to renderer interface

#include <d3d11.h>
#include <memory>
#include <vector>

#include "interface_drawable.h"
#include "../entities/entity_basic.h"
#include "../elements/buffer_vertex.h"

class Texture;
class Model;
class VertexShader;
class PixelShader;
class InputLayout;
class IndexBuffer;
class BasicCamera;
class Scene;
class Presenter;

class TestRender : public IDrawable
{
public:
	TestRender(Presenter* presenter, Scene* scene);
	~TestRender();

	void Draw() override;

private:

	struct TestVertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 uv;
	};

	struct PerFrameVertexBuffer
	{
		DirectX::XMMATRIX viewprojection;
	};

	struct PerObjectBuffer
	{
		DirectX::XMMATRIX world;
	};

	std::unique_ptr<BasicEntity> test_object;
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Model> model;
	std::unique_ptr<InputLayout> input_layout;
	std::unique_ptr<VertexBuffer<TestVertex>> vertex_buffer;
	std::unique_ptr<IndexBuffer> index_buffer;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Scene* scene;
	BasicCamera* camera;
	Presenter* presenter;

	D3D11_BUFFER_DESC per_frame_vertex_buffer_desc;
	D3D11_MAPPED_SUBRESOURCE per_frame_vertex_subresource;
	ID3D11Buffer* per_frame_vertex_buffer;

	D3D11_BUFFER_DESC per_object_buffer_desc;
	D3D11_MAPPED_SUBRESOURCE per_object_subresource;
	ID3D11Buffer* per_object_buffer;

	std::unique_ptr<VertexShader> vertex_shader;
	std::unique_ptr<PixelShader> pixel_shader;

};