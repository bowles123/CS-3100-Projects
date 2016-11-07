// FIFO Queue class.
#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H
#include <vector>
#include "Task.h"
#include "Queue.h"

class FIFO_Queue: public Queue
{
	public:
		/*virtual ~FIFO_Queue()
		{
			delete this;
		}*/

		void insert(Task *tempTask)
		{
			addTask(tempTask);
		}

		Task* getNext()
		{
			return readyQueue.front();
		}
};
#endif