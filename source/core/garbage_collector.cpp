
#include <cassert>

#include "../visuals/elements/buffer_index.h"
#include "../scene/compartments/segment.h"

#include "garbage_collector.h"

constexpr auto QUEUE_SIZE = 100u;

GarbageCollector::GarbageCollector()
{
	segments = std::make_unique<rigtorp::MPMCQueue<Segment*>>(QUEUE_SIZE);
	segment_vertex_buffers = std::make_unique<rigtorp::MPMCQueue<VertexBuffer<SolidBlockVertex>*>>(QUEUE_SIZE);
	index_buffers = std::make_unique<rigtorp::MPMCQueue<IndexBuffer*>>(QUEUE_SIZE);
}

GarbageCollector::~GarbageCollector()
{
}

void GarbageCollector::Update()
{
	Segment* _segment = nullptr;
	while (segments.get()->try_pop(_segment))
	{
		if (!_segment)
			continue;

		if (_segment->draw_mutex.try_lock())
		{
			_segment->draw_mutex.unlock();
			delete _segment;
		}
		else
		{
			if (segments.get()->try_push(_segment))
			{
				continue;
			}
			else
			{
				_segment->draw_mutex.lock();
				_segment->draw_mutex.unlock();
				delete _segment;
			}
		}
	}

	VertexBuffer<SolidBlockVertex>* _vertex_buffer = nullptr;
	while (segment_vertex_buffers.get()->try_pop(_vertex_buffer))
	{
		if (!_vertex_buffer)
			continue;

		delete _vertex_buffer;
	}

	IndexBuffer* _index_buffer = nullptr;
	while (index_buffers.get()->try_pop(_index_buffer))
	{
		if (!_index_buffer)
			continue;

		delete _index_buffer;
	}
}

void GarbageCollector::AddSegment(Segment* target)
{
	assert(target);
	segments->push(target);
}

void GarbageCollector::AddSegmentVertexBuffer(VertexBuffer<SolidBlockVertex>* target)
{
	assert(target);
	segment_vertex_buffers->push(target);
}

void GarbageCollector::AddIndexBuffer(IndexBuffer* target)
{
	assert(target);
	index_buffers->push(target);
}
