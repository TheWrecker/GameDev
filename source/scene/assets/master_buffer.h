
#ifndef MASTER_BUFFER_H
	#define MASTER_BUFFER_H

	#include <d3d11.h>
	#include <DirectXMath.h>
	#include <vector>

	#include "defs_pipeline.h"
	#include "buffer_constant.h"
	#include "buffer_index.h"
	#include "buffer_vertex.h"

	class Model;
	class BasicCamera;
	class Scene;

	class BufferMaster
	{
	public:
		BufferMaster(Scene* presenter);
		~BufferMaster();
		void RebuildDefaults();

		void UpdateDefaultConstant(DefaultConstants target);
		void BindDefaultConstant(DefaultConstants target);

		void BindDefaultObject(DefaultObjects target);
		void BindDefaultIndexBuffer(DefaultObjects target);
		unsigned int GetIndexCount(DefaultObjects target);

		unsigned int GetCurrentSlot(DefaultConstants target);
		unsigned int GetCurrentSlot(DefaultObjects target);

	private:
		void CreateObjectBuffers(unsigned int index, Model* model, bool addNormals = false);

		std::vector<VertexBuffer<DefaultVertexStruct>*> default_vertex_buffers;
		std::vector<VertexBuffer<NormalVertexStruct>*> normal_vertex_buffers;
		std::vector<IndexBuffer*> default_index_buffers;
		std::vector<ConstantBuffer<DefaultConstantStruct>*> default_constant_buffers;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		Scene* scene;
		BasicCamera* camera;
	};

#endif // !MASTER_BUFFER_H
