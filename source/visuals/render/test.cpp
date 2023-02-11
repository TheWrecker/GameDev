
#include <d3dcompiler.h>

#include "../elements/mesh.h"
#include "../elements/model.h"
#include "../elements/texture.h"
#include "../elements/shader_pixel.h"
#include "../elements/shader_vertex.h"
#include "../entities/camera_basic.h"
#include "../elements/input_layout.h"
#include "../elements/buffer_index.h"
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
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/test_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/test_p.hlsl");
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT)
		.Build();

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
	vertex_buffer = std::make_unique<VertexBuffer<TestVertex>>(device, context, _vertexCount);
	std::size_t i = 0;
	//TODO: setup dynamic model lookup
	TestVertex _vertex = {};
	for (i;i<_vertexCount; i++)
	{
		auto& vertex = model->meshes.at(0)->vertices[i];
		_vertex.position = { vertex.x, vertex.y, vertex.z, 1.0f };
		auto& texCoords = model->meshes[0]->texture_coordinates.at(0)->at(i);
		_vertex.uv = { texCoords.x, texCoords.y };
		vertex_buffer->AddVertex(_vertex);
	}
	vertex_buffer->Build();
	//for (auto& normal : model->meshes[0]->normals)
	//{
	//	vertices[i].normal = normal;
	//	i++;
	//}

	std::size_t _indexCount = model->meshes.at(0)->indices.size();
	index_buffer = std::make_unique<IndexBuffer>(device, context, _indexCount);
	for (auto& index : model->meshes.at(0)->indices)
	{
		index_buffer->AddIndex(index);
	}
	index_buffer->Build();
	camera = scene->GetActiveCamera();
}

TestRender::~TestRender()
{
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

	vertex_buffer->Bind();
	index_buffer->Bind();
	context->VSSetConstantBuffers(0, 1, &per_frame_vertex_buffer);
	context->VSSetConstantBuffers(1, 1, &per_object_buffer);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();
	auto _shaderView = texture->GetShaderView();
	context->PSSetShaderResources(0, 1, &_shaderView);

	context->DrawIndexed(index_buffer->GetIndexCount(), 0, 0);
}
