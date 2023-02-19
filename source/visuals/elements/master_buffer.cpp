
#include "mesh.h"
#include "model.h"
#include "../../entities/camera_basic.h"
#include "../scene.h"
#include "../presenter.h"

#include "master_buffer.h"

BufferMaster::BufferMaster(Scene* scene)
	:scene(scene), default_constant_buffers(), default_index_buffers(), default_vertex_buffers()
{
	device = scene->GetPresenter()->GetDevice();
	context = scene->GetPresenter()->GetContext();
	camera = scene->GetActiveCamera();
	RebuildDefaults();
}

void BufferMaster::RebuildDefaults()
{
	if (default_constant_buffers.size() > 0)
	{
		for (auto item : default_constant_buffers)
			delete item;
	}

	if (default_vertex_buffers.size() > 0)
	{
		for (auto item : default_vertex_buffers)
			delete item;
	}

	if (default_index_buffers.size() > 0)
	{
		for (auto item : default_index_buffers)
			delete item;
	}

	default_constant_buffers.clear();
	default_vertex_buffers.clear();
	default_index_buffers.clear();

	default_constant_buffers.resize(static_cast<unsigned int>(DefaultConstants::END_PADDING));
	default_vertex_buffers.resize(static_cast<unsigned int>(DefaultObjects::END_PADDING));
	default_index_buffers.resize(static_cast<unsigned int>(DefaultObjects::END_PADDING));
	unsigned int _index = 0;

	//view projection matrix
	_index = static_cast<unsigned int>(DefaultConstants::VIEW_PROJECTION_MATRIX);
	DefaultConstantStruct _info = { camera->View_Projection_Matrix() };
	default_constant_buffers[_index] = new ConstantBuffer<DefaultConstantStruct>(device, context);
	default_constant_buffers[_index]->Update(_info);

	Model* _model = nullptr;
	//quad buffers
	_index = static_cast<unsigned int>(DefaultObjects::QUAD);
	_model = scene->GetModelManager()->Get("quad");
	CreateObjectBuffers(_index, _model);

	//block buffers
	_index = static_cast<unsigned int>(DefaultObjects::BLOCK);
	_model = scene->GetModelManager()->Get("block");
	CreateObjectBuffers(_index, _model);

	//sphere buffer
	_index = static_cast<unsigned int>(DefaultObjects::SPHERE);
	_model = scene->GetModelManager()->Get("sphere");
	CreateObjectBuffers(_index, _model);
}

BufferMaster::~BufferMaster()
{
	for (auto item : default_constant_buffers)
		delete item;

	for (auto item : default_vertex_buffers)
		delete item;

	for (auto item : default_index_buffers)
		delete item;
}

void BufferMaster::UpdateDefaultConstant(DefaultConstants target)
{
	switch (target)
	{
		case DefaultConstants::VIEW_PROJECTION_MATRIX:
		{
			DefaultConstantStruct _info = { DirectX::XMMatrixTranspose(camera->View_Projection_Matrix()) };
			default_constant_buffers[static_cast<unsigned int>(target)]->Update(_info);
			break;
		}
		default:
			break;
	}
}

void BufferMaster::BindDefaultConstant(DefaultConstants target)
{
	switch (target)
	{
		case DefaultConstants::VIEW_PROJECTION_MATRIX:
		{
			default_constant_buffers[static_cast<unsigned int>(target)]->Bind(BindStage::VERTEX, static_cast<unsigned int>(target));
			break;
		}
		default:
			break;
	}
}

void BufferMaster::UnbindDefaultConstant(DefaultConstants target)
{
	default_constant_buffers[static_cast<unsigned int>(target)]->Unbind();
}

void BufferMaster::BindDefaultObject(DefaultObjects target)
{
	default_vertex_buffers[static_cast<unsigned int>(target)]->Bind(static_cast<unsigned int>(target));
	default_index_buffers[static_cast<unsigned int>(target)]->Bind();
}

void BufferMaster::UnbindDefaultObject(DefaultObjects target)
{
	default_vertex_buffers[static_cast<unsigned int>(target)]->Unbind();
	default_index_buffers[static_cast<unsigned int>(target)]->Unbind();
}

unsigned int BufferMaster::GetIndexCount(DefaultObjects target)
{
	return default_index_buffers[static_cast<unsigned int>(target)]->GetIndexCount();
}

unsigned int BufferMaster::GetCurrentSlot(DefaultConstants target)
{
	return static_cast<unsigned int>(target);
}

unsigned int BufferMaster::GetCurrentSlot(DefaultObjects target)
{
	return static_cast<unsigned int>(target);
}

void BufferMaster::CreateObjectBuffers(unsigned int index, Model* model)
{
	std::size_t _vertexCount = model->meshes.at(0)->vertices.size();
	std::size_t i = 0;
	default_vertex_buffers[index] = new VertexBuffer<DefaultVertexStruct>(device, context, _vertexCount);
	DefaultVertexStruct _vertex = {};
	for (i; i < _vertexCount; i++)
	{
		auto& vertex = model->meshes.at(0)->vertices[i];
		_vertex.position = { vertex.x, vertex.y, vertex.z, 1.0f };
		auto& texCoords = model->meshes[0]->texture_coordinates.at(0)->at(i);
		_vertex.uv = { texCoords.x, texCoords.y };
		default_vertex_buffers[index]->AddVertex(_vertex);
	}
	default_vertex_buffers[index]->Build();

	std::size_t _indexCount = model->meshes.at(0)->indices.size();
	default_index_buffers[index] = new IndexBuffer(device, context, _indexCount);
	for (auto& element : model->meshes.at(0)->indices)
	{
		default_index_buffers[index]->AddIndex(element);
	}
	default_index_buffers[index]->Build();
}
