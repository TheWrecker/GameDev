
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "buffer_constant.h"
#include "buffer_index.h"
#include "buffer_vertex.h"

class Model;
class BasicCamera;
class Scene;

//we dont use first and last slots

enum class DefaultConstants : unsigned int
{
	//14 slots total
	UNDEFINED = 0,
	VIEW_PROJECTION_MATRIX = 12,
	END_PADDING = 13 //D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT (14)
};

enum class DefaultObjects : unsigned int
{
	//32 slots total
	UNDEFINED = 0,
	BLOCK = 29,
	SPHERE = 30,
	END_PADDING = 31 //D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT (32)
};

class BufferMaster
{
public:
	BufferMaster(Scene* presenter);
	~BufferMaster();
	void RebuildDefaults();

	void UpdateDefaultConstant(DefaultConstants target);
	void BindDefaultConstant(DefaultConstants target);
	void UnbindDefaultConstant(DefaultConstants target);

	void BindDefaultObject(DefaultObjects target);
	void UnbindDefaultObject(DefaultObjects target);
	unsigned int GetIndexCount(DefaultObjects target);

	unsigned int GetCurrentSlot(DefaultConstants target);
	unsigned int GetCurrentSlot(DefaultObjects target);

private:
	void CreateObjectBuffers(unsigned int index, Model* model);

	struct DefaultVertexStruct
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 uv;
	};

	struct DefaultConstantStruct
	{
		DirectX::XMMATRIX matrix;
	};

	std::vector<VertexBuffer<DefaultVertexStruct>*> default_vertex_buffers;
	std::vector<IndexBuffer*> default_index_buffers;
	std::vector<ConstantBuffer<DefaultConstantStruct>*> default_constant_buffers;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Scene* scene;
	BasicCamera* camera;
};
