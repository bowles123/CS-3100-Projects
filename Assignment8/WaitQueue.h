// WaitQueue class.
#ifndef WAITQUEUE_H
#define WAITQUEUE_H
#include <vector>
#include "Task.h"
#include "IO_Burst.h"

class WaitQueue
{
	private:
		int deviceNum;
		bool avail;
		std::vector<Task*> wait;
	
	public:
		WaitQueue(int device, bool available)
		{
			deviceNum = device;
			avail = available;
		}

		/*~WaitQueue()
		{
			delete this;
		}*/

		Task* pop()
		{
			Task *task = wait.front();
			wait.erase(wait.begin());
			return task;
		}
	
		void push(Task *t)
		{
			wait.push_back(t);
		}
		
		void setDeviceNum(int num)
		{
			deviceNum = num;
		}
		
		int getDeviceNum()
		{
			return deviceNum;
		}
		
		void setAvail(bool available)
		{
			avail = available;
		}
		
		bool isAvailable()
		{
			if (wait.empty() && avail)
				return true;
			return false;
				
		}
};
#endif