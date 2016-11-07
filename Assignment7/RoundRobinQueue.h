// Ready queue for round robin algorithm
#ifndef ROUNDROBINQUEUE_H
#define ROUNDROBINQUEUE_H
#include <vector>
#include "Task.h"
#include "Queue.h"

class RoundRobinQueue: Queue
{
	void insert()
	{

	}

	Task* getNext()
	{
		return readyQueue.front();
	}
};
#endif