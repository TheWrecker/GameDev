
#pragma once

//TODO: change to renderer interface

#include <d3d11.h>
#include <memory>
#include <vector>

#include "interface_drawable.h"
#include "../entities/entity_basic.h"

class Texture;
class Model;
class VertexShader;
class PixelShader;
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
	std::unique_ptr<BasicEntity> test_object;
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Model> model;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Scene* scene;
	BasicCamera* camera;
	Presenter* presenter;

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

	std::vector<TestVertex> vertices;
	std::vector<UINT> indices;
	UINT
		vertex_stride,
		vertex_offset;

	ID3D11Buffer
		* vertex_buffer,
		* index_buffer;

	D3D11_BUFFER_DESC
		vertex_buffer_desc,
		index_buffer_desc;

	D3D11_SUBRESOURCE_DATA
		vertex_subresource_data,
		index_subresource_data;

	ID3D11InputLayout* input_layout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout_desc;

	D3D11_BUFFER_DESC per_frame_vertex_buffer_desc;
	D3D11_MAPPED_SUBRESOURCE per_frame_vertex_subresource;
	ID3D11Buffer* per_frame_vertex_buffer;

	D3D11_BUFFER_DESC per_object_buffer_desc;
	D3D11_MAPPED_SUBRESOURCE per_object_subresource;
	ID3D11Buffer* per_object_buffer;

	std::unique_ptr<VertexShader> vertex_shader;
	std::unique_ptr<PixelShader> pixel_shader;

};