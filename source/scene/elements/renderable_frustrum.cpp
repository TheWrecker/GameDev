
#include "../utils/ray.h"
#include "../visuals/elements/buffer_index.h"
#include "../visuals/elements/buffer_vertex.h"
#include "../camera/camera_basic.h"
#include "../scene.h"

#include "renderable_frustrum.h"

constexpr unsigned int RENDERABLE_FRUSTRUM_VERTEX_COUNT = 8u;
constexpr unsigned int RENDERABLE_FRUSTRUM_INDEX_COUNT = 24u;

const unsigned int Indices[RENDERABLE_FRUSTRUM_INDEX_COUNT] =
{
	// Near plane lines
	0, 1,
	1, 2,
	2, 3,
	3, 0,

	// Sides
	0, 4,
	1, 5,
	2, 6,
	3, 7,

	// Far plane lines
	4, 5,
	5, 6,
	6, 7,
	7, 4
};

RenderableFrustrum::RenderableFrustrum(Scene* scene)
    :scene(scene)
{
	vertex_buffer->Initialize(scene->GetDevice(), scene->GetContext(), RENDERABLE_FRUSTRUM_VERTEX_COUNT);
    index_buffer->Initialize(scene->GetDevice(), scene->GetContext(), RENDERABLE_FRUSTRUM_INDEX_COUNT);
	color = { 1.0f, 1.0f, 1.0f, 1.0f }; //white

	for (unsigned int _i = 0; _i < RENDERABLE_FRUSTRUM_INDEX_COUNT; _i++)
	{
		index_buffer->AddIndex(Indices[_i]);
	}
	index_buffer->Build();
}

RenderableFrustrum::~RenderableFrustrum()
{
}

bool RenderableFrustrum::Initialize()
{
	return UpdateToCamera();
}

bool RenderableFrustrum::UpdateToCamera()
{
	auto _camera = scene->GetActiveCamera();

	if (!_camera)
		return false;

	SetPosition(_camera->Position().x, _camera->Position().y, _camera->Position().z);
	DirectX::XMFLOAT4X4 _proj = {};
	DirectX::XMStoreFloat4x4(&_proj, _camera->Projection_Matrix());
	frustrum.CalculateFrustrum(_camera->View_Matrix(), _proj, 30.0f);
	//frustrum.CalculateFrustrum(_camera->View_Projection_Matrix());
	CalculateCorners();
	projector.SetPosition(_camera->Position().x, _camera->Position().y, _camera->Position().z);
	projector.SetDirection(_camera->Direction().x, _camera->Direction().y, _camera->Direction().z);
	RebuildBuffers();

	return true;
}

void RenderableFrustrum::SetColor(DirectX::XMFLOAT4 value)
{
	color = value;
}

void RenderableFrustrum::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	// world matrix is implicit in the view matrix, so we store an identity matrix
	world_matrix = DirectX::XMMatrixIdentity();
}

void RenderableFrustrum::CalculateCorners()
{
	Ray ray = ComputeIntersectionLine(frustrum.Near_Vector(), frustrum.Left_Vector());
	DirectX::XMStoreFloat3(&corners[0], ComputeIntersectionPoint(frustrum.Top_Vector(), ray));
	DirectX::XMStoreFloat3(&corners[3], ComputeIntersectionPoint(frustrum.Bottom_Vector(), ray));

	ray = ComputeIntersectionLine(frustrum.Right_Vector(), frustrum.Near_Vector());
	DirectX::XMStoreFloat3(&corners[1], ComputeIntersectionPoint(frustrum.Top_Vector(), ray));
	DirectX::XMStoreFloat3(&corners[2], ComputeIntersectionPoint(frustrum.Bottom_Vector(), ray));

	ray = ComputeIntersectionLine(frustrum.Left_Vector(), frustrum.Far_Vector());
	DirectX::XMStoreFloat3(&corners[4], ComputeIntersectionPoint(frustrum.Top_Vector(), ray));
	DirectX::XMStoreFloat3(&corners[7], ComputeIntersectionPoint(frustrum.Bottom_Vector(), ray));

	ray = ComputeIntersectionLine(frustrum.Far_Vector(), frustrum.Right_Vector());
	DirectX::XMStoreFloat3(&corners[5], ComputeIntersectionPoint(frustrum.Top_Vector(), ray));
	DirectX::XMStoreFloat3(&corners[6], ComputeIntersectionPoint(frustrum.Bottom_Vector(), ray));
}

void RenderableFrustrum::RebuildBuffers()
{
	vertex_buffer.get()->Clear();
	for (unsigned int _i = 0; _i < RENDERABLE_FRUSTRUM_VERTEX_COUNT; _i++)
	{
		vertex_buffer->AddVertex(ColorVertexStruct(corners[_i], color));
	}
	vertex_buffer->Build();
}
