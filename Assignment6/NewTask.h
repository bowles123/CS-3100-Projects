// NewTask class.
#ifndef NEWTASK_H
#define NEWTASK_H
#include "Event.h"
#include "Task.h"
#include <string>

class NewTask: public Event
{
	private:
		Task *newTask;
	
	public:
		NewTask(Task *task) : Event(task->getStartTime())
		{
			newTask = task;
		}

		/*virtual ~NewTask()
		{
			delete this;
		}*/
		
		std::string eventType(const Event &object) {return "NewTaskEvent";}
};
#endif