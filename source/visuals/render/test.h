
#pragma once

#include <d3d11.h>
#include <memory>
#include <vector>

#include "interface_drawable.h"
#include "../entities/entity_basic.h"

class TestRender : public IDrawable
{
public:
	TestRender(ID3D11Device* device, ID3D11DeviceContext* context);
	~TestRender();

	void Draw() override;

private:
	std::unique_ptr<BasicEntity> test_object;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	struct TestVertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
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

	ID3D11VertexShader* vertex_shader;
	ID3D11PixelShader* pixel_shader;

};