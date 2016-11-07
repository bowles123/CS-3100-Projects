// Ready queue for the approximate shortest job first algorithm.
#ifndef APPROXIMATESHORTESTJOBQUEUE_H
#define APPROXIMATESHORTESTJOBQUEUE_H
#include <vector>
#include "Task.h"
#include "Queue.h"

class ApproximateShortestJobQueue: public Queue
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