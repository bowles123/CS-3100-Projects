// SimulationDoneEvent class.
#ifndef SIMULATIONDONEEVENT_H
#define SIMULATIONDONEEVENT_H
#include "Event.h"

class SimulationDoneEvent: public Event
{
	public:
		SimulationDoneEvent(double time) : Event(time) { }

		/*virtual ~SimulationDoneEvent()
		{
			delete this;
		}*/

		std::string eventType(const Event &object) {return "SimulationDoneEvent";}
};
#endif