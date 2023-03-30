
#include <cassert>

#include "../scene/compartments/segment.h"

#include "garbage_collector.h"

GarbageCollector::GarbageCollector()
{
}

GarbageCollector::~GarbageCollector()
{
}

void GarbageCollector::Update()
{
	if (segments.size() > 0)
	{
		auto _seg = segments.front();
		if (!_seg)
			segments.pop_front();

		if (_seg->draw_mutex.try_lock())
		{
			_seg->draw_mutex.unlock();
			segments.remove(_seg);
			delete _seg;
		}
	}
}

void GarbageCollector::AddSegment(Segment* target)
{
	assert(target);
	segments.push_back(target);
}
