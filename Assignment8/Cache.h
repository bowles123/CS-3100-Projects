#ifndef CACHE_H
#define CACHE_H
#include <queue>

class Cache
{
	private:
		int memorySize;
		std::queue<int> memory;

	public:
		Cache(int size)
		{
			memorySize = size;
		}

		Cache()
		{
			memorySize = 2;
		}
		
		int getMemorySize()
		{
			return memorySize;
		}
		
		bool isFull()
		{
			if(memory.size() == memorySize)
				return true;
			return false;
		}

		bool isAvailable(int page)
		{
			std::queue<int> temp = memory;
			for (int i = 0; i < temp.size(); i++)
			{
				if (temp.front() == page)
					return true;
				temp.pop();
			}
			return false;
		}
		
		void replace(int page)
		{
			memory.pop();
			memory.push(page);
		}

		void add(int page)
		{
			if (memory.size() < memorySize)
				memory.push(page);
			else
				replace(page);
		}
};
#endif