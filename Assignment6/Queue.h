// Abstract class Queue
#ifndef QUEUE_H
#define QUEUE_H
#include <vector>
#include <queue>
#include "Task.h"

class Queue 
{
	protected:
		std::queue<Task*> readyQueue;

	public:
		/*virtual ~Queue()
		{
			delete this;  
		}*/

		virtual void insert(Task *t) = 0;
		virtual Task* getNext() = 0;
	
		std::queue<Task*> getQueue()
		{
			return readyQueue;
		}
	
		void addTask (Task* t)
		{
			readyQueue.push(t);
		}
};
#endif	
		
	