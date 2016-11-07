// IO_DoneEvent class.
#ifndef IO_DONEEVENT_H
#define IO_DONEEVENT_H
#include "Event.h"

class IO_DoneEvent: public Event
{
	public:
		IO_DoneEvent(double time) : Event(time) { }

		/*virtual ~IO_DoneEvent()
		{
			delete this;
		}*/

		std::string eventType(const Event &object) {return "IO_DoneEvent";}
};
#endif