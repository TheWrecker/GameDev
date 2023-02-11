
#include <d3dcompiler.h>

#include "../elements/mesh.h"
#include "../elements/model.h"
#include "../elements/texture.h"
#include "../elements/pixel_shader.h"
#include "../elements/vertex_shader.h"
#include "../entities/camera_basic.h"
#include "../scene.h"
#include "../presenter.h"
#include "util_funcs.h"

#include "test.h"

TestRender::TestRender(Presenter* presenter, Scene* scene)
	:scene(scene), presenter(presenter)
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
	test_object = std::make_unique<BasicEntity>();
	vertex_stride = sizeof(TestVertex);
	vertex_offset = 0;
	input_layout_desc.push_back(
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	input_layout_desc.push_back(
		{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 });


	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/test_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/test_p.hlsl");
	DXAssert(device->CreateInputLayout(&input_layout_desc.at(0), input_layout_desc.size(), vertex_shader->GetByteCode(), vertex_shader->GetSize(), &input_layout));

	model = std::make_unique<Model>(L"assets/models/sphere.obj", true);
	texture = std::make_unique<Texture>(presenter, L"assets/textures/earth.dds");

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

	std::size_t _vertexCount = model->meshes.at(0)->vertices.size();
	vertices.resize(_vertexCount);

	size_t i = 0;
	//TODO: setup dynamic model lookup
	for (auto& vertex : model->meshes.at(0)->vertices)
	{
		vertices[i].position = { vertex.x, vertex.y, vertex.z, 1.0f };
		i++;
	}

	i = 0;
	for (auto& textureCoord : *model->meshes[0]->texture_coordinates.at(0))
	{
		vertices[i].uv = { textureCoord.x, textureCoord.y };
		i++;
	}

	i = 0;
	//for (auto& normal : model->meshes[0]->normals)
	//{
	//	vertices[i].normal = normal;
	//	i++;
	//}

	std::size_t indexCount = model->meshes.at(0)->indices.size();
	indices.resize(indexCount);

	i = 0;
	for (auto& index : model->meshes.at(0)->indices)
	{
		indices[i] = index;
		i++;

	}

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
}

TestRender::~TestRender()
{
	DXRelease(vertex_buffer);
	DXRelease(index_buffer);
	DXRelease(input_layout);
	DXRelease(per_frame_vertex_buffer);
	DXRelease(per_object_buffer);
}

void TestRender::Draw()
{
	DirectX::XMMATRIX viewProjectionMatrixTransposed =	DirectX::XMMatrixTranspose(camera->View_Projection_Matrix());
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
	vertex_shader->Apply();
	pixel_shader->Apply();
	auto _shaderView = texture->GetShaderView();
	context->PSSetShaderResources(0, 1, &_shaderView);

	context->DrawIndexed(indices.size(), 0, 0);
}
