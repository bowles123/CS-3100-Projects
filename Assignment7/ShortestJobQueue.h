// Ready queue for the shortest job first algorithm.
#ifndef SHORTESTJOBQUEUE_H
#define SHORTESTJOBQUEUE_H
#include <vector>
#include "Task.h"
#include "Queue.h"

class ShortestJobQueue: Queue
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