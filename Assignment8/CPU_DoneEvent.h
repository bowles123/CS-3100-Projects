// CPU Done event class.
#ifndef CPU_DONEEVENT_H
#define CPU_DONEEVENT_H
#include "Event.h"
#include "Task.h"

class CPU_DoneEvent: public Event
{
	public:
		CPU_DoneEvent(double time) : Event(time) { }

		/*virtual ~CPU_DoneEvent()
		{
			delete this;
		}*/

		std::string eventType(const Event &object) {return "CPU_DoneEvent";}

};
#endif