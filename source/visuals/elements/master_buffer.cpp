
#include "mesh.h"
#include "model.h"
#include "manager_model.h"
#include "../entities/sun.h"
#include "../entities/camera_basic.h"
#include "../scene.h"
#include "../presenter.h"

#include "master_buffer.h"

BufferMaster::BufferMaster(Scene* scene)
	:scene(scene), default_constant_buffers(), default_index_buffers(), default_vertex_buffers(), normal_vertex_buffers()
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

	if (normal_vertex_buffers.size() > 0)
	{
		for (auto item : normal_vertex_buffers)
			delete item;
	}

	if (default_index_buffers.size() > 0)
	{
		for (auto item : default_index_buffers)
			delete item;
	}

	default_constant_buffers.clear();
	default_vertex_buffers.clear();
	normal_vertex_buffers.clear();
	default_index_buffers.clear();

	default_constant_buffers.resize(static_cast<unsigned int>(DefaultConstants::END_PADDING));
	default_vertex_buffers.resize(static_cast<unsigned int>(DefaultObjects::END_PADDING));
	normal_vertex_buffers.resize(static_cast<unsigned int>(DefaultObjects::END_PADDING));
	default_index_buffers.resize(static_cast<unsigned int>(DefaultObjects::END_PADDING));
	unsigned int _index = 0;

	//sun light data
	_index = static_cast<unsigned int>(DefaultConstants::SUN_LIGHT_DATA);
	DefaultConstantStruct _info = { scene->GetSun()->GetLightInfo() };
	default_constant_buffers[_index] = new ConstantBuffer<DefaultConstantStruct>(device, context);
	default_constant_buffers[_index]->Update(_info);

	//view projection matrix
	_index = static_cast<unsigned int>(DefaultConstants::VIEW_PROJECTION_MATRIX);
	_info = { camera->View_Projection_Matrix() };
	default_constant_buffers[_index] = new ConstantBuffer<DefaultConstantStruct>(device, context);
	default_constant_buffers[_index]->Update(_info);

	Model* _model = nullptr;
	//quad buffers without normals
	_index = static_cast<unsigned int>(DefaultObjects::QUAD);
	_model = scene->GetModelManager()->Get("quad");
	CreateObjectBuffers(_index, _model);

	//block buffers without normals
	_index = static_cast<unsigned int>(DefaultObjects::BLOCK);
	_model = scene->GetModelManager()->Get("block");
	CreateObjectBuffers(_index, _model);

	//sphere buffer without normals
	_index = static_cast<unsigned int>(DefaultObjects::SPHERE);
	_model = scene->GetModelManager()->Get("sphere");
	CreateObjectBuffers(_index, _model);

	_model = nullptr;
	//quad buffers with normals
	_index = static_cast<unsigned int>(DefaultObjects::QUAD_NORMAL);
	_model = scene->GetModelManager()->Get("quad");
	CreateObjectBuffers(_index, _model, true);

	//block buffers with normals
	_index = static_cast<unsigned int>(DefaultObjects::BLOCK_NORMAL);
	_model = scene->GetModelManager()->Get("block");
	CreateObjectBuffers(_index, _model, true);

	//sphere buffer with normals
	_index = static_cast<unsigned int>(DefaultObjects::SPHERE_NORMAL);
	_model = scene->GetModelManager()->Get("sphere");
	CreateObjectBuffers(_index, _model, true);
}

BufferMaster::~BufferMaster()
{
	for (auto item : default_constant_buffers)
		delete item;

	for (auto item : default_vertex_buffers)
		delete item;

	for (auto item : normal_vertex_buffers)
		delete item;

	for (auto item : default_index_buffers)
		delete item;
}

void BufferMaster::UpdateDefaultConstant(DefaultConstants target)
{
	auto _index = static_cast<unsigned int>(target);
	switch (target)
	{
		case DefaultConstants::SUN_LIGHT_DATA:
		{
			DefaultConstantStruct _info = { scene->GetSun()->GetLightInfo()};
			default_constant_buffers[_index]->Update(_info);
			break;
		}
		case DefaultConstants::VIEW_PROJECTION_MATRIX:
		{
			DefaultConstantStruct _info = { DirectX::XMMatrixTranspose(camera->View_Projection_Matrix()) };
			default_constant_buffers[_index]->Update(_info);
			break;
		}
		default:
			break;
	}
}

void BufferMaster::BindDefaultConstant(DefaultConstants target)
{
	auto _index = static_cast<unsigned int>(target);
	switch (target)
	{
		case DefaultConstants::VIEW_PROJECTION_MATRIX:
		{
			default_constant_buffers[_index]->Bind(BindStage::VERTEX, _index);
			break;
		}
		case DefaultConstants::SUN_LIGHT_DATA:
		{
			default_constant_buffers[_index]->Bind(BindStage::PIXEL, _index);
			break;
		}
		default:
			break;
	}
}

void BufferMaster::BindDefaultObject(DefaultObjects target)
{
	auto _index = static_cast<unsigned int>(target);
	switch (target)
	{
		case DefaultObjects::QUAD_NORMAL:
		case DefaultObjects::BLOCK_NORMAL:
		case DefaultObjects::SPHERE_NORMAL:
		{
			normal_vertex_buffers[_index]->Bind(_index);
			break;
		}
		case DefaultObjects::QUAD:
		case DefaultObjects::BLOCK:
		case DefaultObjects::SPHERE:
		{
			default_vertex_buffers[_index]->Bind(_index);
			break;
		}
		default:
			break;
	}
	default_index_buffers[_index]->Bind();
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

void BufferMaster::CreateObjectBuffers(unsigned int index, Model* model, bool addNormals)
{
	std::size_t _vertexCount = model->meshes.at(0)->vertices.size();
	std::size_t i = 0;
	if (addNormals)
	{
		normal_vertex_buffers[index] = new VertexBuffer<NormalVertexStruct>(device, context, _vertexCount);
		NormalVertexStruct _vertex = {};
		for (i; i < _vertexCount; i++)
		{
			_vertex.position = model->meshes.at(0)->vertices[i];
			auto& texCoords = model->meshes[0]->texture_coordinates.at(0)->at(i);
			_vertex.uv = { texCoords.x, texCoords.y };
			if (model->meshes.at(0)->normals.size() > 0)
				_vertex.normal = model->meshes.at(0)->normals[i];
			normal_vertex_buffers[index]->AddVertex(_vertex);
		}
		normal_vertex_buffers[index]->Build();
	}
	else
	{
		default_vertex_buffers[index] = new VertexBuffer<DefaultVertexStruct>(device, context, _vertexCount);
		DefaultVertexStruct _vertex = {};
		for (i; i < _vertexCount; i++)
		{
			_vertex.position = model->meshes.at(0)->vertices[i];
			auto& texCoords = model->meshes[0]->texture_coordinates.at(0)->at(i);
			_vertex.uv = { texCoords.x, texCoords.y };
			default_vertex_buffers[index]->AddVertex(_vertex);
		}
		default_vertex_buffers[index]->Build();
	}

	std::size_t _indexCount = model->meshes.at(0)->indices.size();
	default_index_buffers[index] = new IndexBuffer(device, context, _indexCount);
	for (auto& element : model->meshes.at(0)->indices)
	{
		default_index_buffers[index]->AddIndex(element);
	}
	default_index_buffers[index]->Build();
}
