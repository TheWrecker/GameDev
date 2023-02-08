
#include <d3dcompiler.h>

#include "../entities/camera_basic.h"
#include "../scene.h"
#include "util_funcs.h"

#include "test.h"

TestRender::TestRender(ID3D11Device* device, ID3D11DeviceContext* context, Scene* scene)
	:device(device), context(context), scene(scene)
{
	test_object = std::make_unique<BasicEntity>();
	vertex_stride = sizeof(TestVertex);
	vertex_offset = 0;
	input_layout_desc.push_back(
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	input_layout_desc.push_back(
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 });

	UINT _shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	_shaderFlags |= D3DCOMPILE_DEBUG;
	_shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob* _blob_v;
	ID3DBlob* _blob_p;
	DXAssert(D3DCompileFromFile(L"source/visuals/shaders/test_v.hlsl", nullptr, nullptr, "main", "vs_5_0", _shaderFlags, 0, &_blob_v, nullptr));
	DXAssert(device->CreateVertexShader(_blob_v->GetBufferPointer(), _blob_v->GetBufferSize(), 0, &vertex_shader));

	DXAssert(D3DCompileFromFile(L"source/visuals/shaders/test_p.hlsl", nullptr, nullptr, "main", "ps_5_0", _shaderFlags, 0, &_blob_p, nullptr));
	DXAssert(device->CreatePixelShader(_blob_p->GetBufferPointer(), _blob_p->GetBufferSize(), 0, &pixel_shader));

	DXAssert(device->CreateInputLayout(&input_layout_desc.at(0), input_layout_desc.size(), _blob_v->GetBufferPointer(), _blob_v->GetBufferSize(), &input_layout));

	//setup constant buffers
	ZeroMemory(&per_frame_vertex_buffer_desc, sizeof(per_frame_vertex_buffer_desc));
	per_frame_vertex_buffer_desc.ByteWidth = sizeof(PerFrameVertexBuffer);
	per_frame_vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	per_frame_vertex_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	per_frame_vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DXAssert(device->CreateBuffer(&per_frame_vertex_buffer_desc, 0, &per_frame_vertex_buffer));

	ZeroMemory(&per_object_buffer_desc, sizeof(per_object_buffer_desc));
	per_object_buffer_desc.ByteWidth = sizeof(PerObjectBuffer);
	per_object_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	per_object_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	per_object_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DXAssert(device->CreateBuffer(&per_object_buffer_desc, 0, &per_object_buffer));

	vertices.push_back({ DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) });
	vertices.push_back({ DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) });
	vertices.push_back({ DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));
	vertex_buffer_desc.ByteWidth = sizeof(TestVertex) * vertices.size();
	vertex_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&vertex_subresource_data, sizeof(vertex_subresource_data));
	vertex_subresource_data.pSysMem = &vertices.at(0);
	DXAssert(device->CreateBuffer(&vertex_buffer_desc, &vertex_subresource_data, &vertex_buffer));

	ZeroMemory(&index_buffer_desc, sizeof(index_buffer_desc));
	index_buffer_desc.ByteWidth = sizeof(UINT) * indices.size();
	index_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&index_subresource_data, sizeof(index_subresource_data));
	index_subresource_data.pSysMem = &indices.at(0);
	DXAssert(device->CreateBuffer(&index_buffer_desc, &index_subresource_data, &index_buffer));

	camera = scene->GetActiveCamera();

	_blob_v->Release();
	_blob_p->Release();
}

TestRender::~TestRender()
{
}

void TestRender::Draw()
{
	DirectX::XMMATRIX viewProjectionMatrixTransposed =	DirectX::XMMatrixTranspose(camera->ViewProjectionMatrix());
	DirectX::XMMATRIX worldMatrixTransposed = DirectX::XMMatrixTranspose(test_object->World_Matrix());

	ZeroMemory(&per_frame_vertex_subresource, sizeof(per_frame_vertex_subresource));
	context->Map(per_frame_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &per_frame_vertex_subresource);
	reinterpret_cast<PerFrameVertexBuffer*>(per_frame_vertex_subresource.pData)->viewprojection = viewProjectionMatrixTransposed;
	context->Unmap(per_frame_vertex_buffer, 0);

	ZeroMemory(&per_object_subresource, sizeof(per_object_subresource));
	context->Map(per_object_buffer, 0, D3D11_MAP_WRITE_DISCARD,	0, &per_object_subresource);
	reinterpret_cast<PerObjectBuffer*>(per_object_subresource.pData)->world = worldMatrixTransposed;
	context->Unmap(per_object_buffer, 0);

	context->IASetVertexBuffers(0, 1, &vertex_buffer, &vertex_stride, &vertex_offset);
	context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
	context->VSSetConstantBuffers(0, 1, &per_frame_vertex_buffer);
	context->VSSetConstantBuffers(1, 1, &per_object_buffer);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(input_layout);
	context->VSSetShader(vertex_shader, 0, 0);
	context->PSSetShader(pixel_shader, 0, 0);

	context->DrawIndexed(indices.size(), 0, 0);
}
