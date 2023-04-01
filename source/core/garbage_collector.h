
#ifndef GARBAGE_COLLECTOR_H
	#define GARBAGE_COLLECTOR_H

	#include <memory>

	#include "../external/MPMC Queue.h"

	#include "interface_service.h"
	#include "defs_pipeline.h"
	#include "../visuals/elements/buffer_vertex.h"

	class Segment;
	class IndexBuffer;

	class GarbageCollector : public IService
	{
	public:
		GarbageCollector();
		~GarbageCollector();

		void Update() override;

		void AddSegment(Segment* target);
		void AddSegmentVertexBuffer(VertexBuffer<SolidBlockVertex>* target);

		void AddIndexBuffer(IndexBuffer* target);

	private:

		std::unique_ptr<rigtorp::MPMCQueue<Segment*>> segments;
		std::unique_ptr<rigtorp::MPMCQueue<VertexBuffer<SolidBlockVertex>*>> segment_vertex_buffers;
		std::unique_ptr<rigtorp::MPMCQueue<IndexBuffer*>> index_buffers;
		//TODO: run in periodic task instead

	};


#endif // !GARBAGE_COLLECTOR_H


