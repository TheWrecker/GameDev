
#ifndef GARBAGE_COLLECTOR_H
	#define GARBAGE_COLLECTOR_H

	#include <list>

	#include "interface_service.h"
	//#include "executor.h"

	class Segment;

	class GarbageCollector : public IService
	{
	public:
		GarbageCollector();
		~GarbageCollector();

		void Update() override;

		void AddSegment(Segment* target);

	private:

		std::list<Segment*> segments;
		//TODO: run in periodic task instead

	};


#endif // !GARBAGE_COLLECTOR_H


