
#ifndef PROXY_H
	#define PROXY_H

	#include <DirectXMath.h>
	#include <memory>

	#include "defs_pipeline.h"
	#include "../visuals/elements/buffer_vertex.h"

	class IndexBuffer;

	class Proxy
	{
	public:
		Proxy(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		virtual ~Proxy();

		virtual void RebuildBuffers() = 0;

		virtual void SetPosition(float x, float y, float z);
		virtual const DirectX::XMFLOAT3& GetPosition() const;
		virtual const DirectX::XMMATRIX& World_Matrix() const;

		VertexBuffer<ColorVertexStruct>* GetVertexBuffer();
		IndexBuffer* GetIndexBuffer();

	protected:
		DirectX::XMFLOAT3 position;
		DirectX::XMMATRIX world_matrix;

		std::unique_ptr<VertexBuffer<ColorVertexStruct>> vertex_buffer;
		std::unique_ptr<IndexBuffer> index_buffer;
	};

#endif // !PROXY_H

