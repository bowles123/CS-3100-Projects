// Brian Bowles, Assignment 6, February 20 2015.
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <numeric> 
#include <utility>
#include <functional>
#include <random>
#include "Task.h"
#include "CPU_Burst.h"
#include "IO_Burst.h"
#include "Event.h"
#include "WaitQueue.h"
#include "Burst.h"
#include "Queue.h"
#include "FIFO_Queue.h"
#include "NewTask.h"
#include "SimulationDoneEvent.h"
#include "CPU_DoneEvent.h"
#include "IO_DoneEvent.h"
#include "Cache.h"

class Compare 
{
	public:
		bool operator()(Event* time1, Event* time2)
		{
			if (time1->getTime() > time2->getTime())
				return true;
			return false;
		}
};

// Function to calculate the mean of values in a container.
double average(std::vector<double> const &c)
{
	return std::accumulate(c.begin(), c.end(), 0) / c.size();
}

// Function to calculate the standard Deviation of values in a container.
double stdDev(std::vector<double> const &c)
{
	std::vector<double> tempContainer(c.size());
	auto squares = std::transform(c.begin(), c.end(), tempContainer.begin(), [&](double a){return a*a; });
	return std::accumulate(tempContainer.begin(), tempContainer.end(), 0) / tempContainer.size();
}

// Generates the number of bursts for a given task.
int generateNumBursts()
{
	std::random_device randDevice;
	std::mt19937 mt (randDevice());
	std::uniform_int_distribution<int> dist(3, 10);
	return dist(mt);
}

int generatePageNumber(int memorySize)
{
	std::random_device randDevice;
	std::mt19937 mt(randDevice());
	std::uniform_int_distribution<int> dist(1, memorySize);
	return dist(mt);
}

double generateBurstTime()
{
	double num;

	do
	{
		std::random_device randDevice;
		std::mt19937 mt(randDevice());
		std::normal_distribution<double> norm(5.2, 2.1);
		num = norm(mt);
	} while (num <= 0);
	return num;
}

double generateEndTime()
{
	double num;

	do
	{
		std::random_device randDevice;
		std::mt19937 mt(randDevice());
		std::normal_distribution<double> norm(5000, 100);
		num = norm(mt);
	} while (num <= 0);
	return num;
}

// Generates a vector of bursts.
std::vector<Burst*> burstGenerate(int size)
{
	std::vector<Burst*> burstList;
	
	if (size % 2 == 0)
		size -= size;

	for (int i = 1; i < size + 1; i++)
		if (i % 2 == 1)
			burstList.push_back(new CPU_Burst(generateBurstTime()));
		else
			burstList.push_back(new IO_Burst(generateBurstTime()));
	return burstList;
}

// Generates a list of tasks.
std::vector<Task*> listGenerate(int size)
{
	std::vector<Task*> lst;
	int burstListSize = generateNumBursts();
	
	for (int i = 1; i < size + 1; i++)
		lst.push_back(new Task());
	for (int i = 0; i < lst.size(); i++)
		lst[i]->setBursts(burstGenerate(burstListSize));
	return lst;
}

int main()
{
	std::vector<double> responseTime;
	std::vector<Task*> tasks = listGenerate(generateNumBursts());
	std::priority_queue<Event*, std::vector<Event*>, Compare> eventQueue;
	std::vector<WaitQueue*> waits;
	FIFO_Queue readyQueue;
	Task *currentTask;
	Burst *currentBurst;
	Event *currentEvent;
	SimulationDoneEvent *done = new SimulationDoneEvent(generateEndTime());
	Cache *cache;
	double time = 1.5, timePenalty;
	std::vector<Event*> waitQueue;
	bool IOAvailable = true, CPUAvailable = true;
	int cacheSize, burstOrigSize, memorySize, pageNumber, faults = 0;

	std::cout << "Please enter the size of memory in terms of pages: ";
	std::cin >> memorySize;
	std::cout << "Please enter the time penalty for a page fault: ";
	std::cin >> timePenalty;
	std::cout << "Please enter the size of cache: ";
	std::cin >> cacheSize;
	cache = new Cache(cacheSize);

	for (int i = 0; i < tasks.size(); i++)
		eventQueue.emplace(new NewTask(tasks[i], time));
	eventQueue.emplace(done);

	time += 1.3;

	do
	{
		currentTask = tasks.front();
		currentEvent = eventQueue.top();
		eventQueue.pop();

		if (currentTask->getBurst())
		{
			tasks.erase(tasks.begin(), tasks.begin());
			tasks.push_back(currentTask);
		}
		else
			tasks.erase(tasks.begin(), tasks.begin());

		time += 0.7;

		if (currentEvent->eventType(*currentEvent) == "CPU_DoneEvent")
		{
			CPUAvailable = true;

			if (!readyQueue.isEmpty())
				eventQueue.emplace(new CPU_DoneEvent(time));

			if (IOAvailable == false)
				waitQueue.push_back(currentEvent);
			else
			{
				eventQueue.emplace(new IO_DoneEvent(time));
				std::cout << "It's time: " << time << " and CPU has finished." << std::endl;
				IOAvailable = false;
			}

			time += 1.1;
		}
		else
			if (currentEvent->eventType(*currentEvent) == "IO_DoneEvent")
			{
				pageNumber = generatePageNumber(memorySize);
				IOAvailable = true;

				if (waitQueue.size() != 0)
					eventQueue.emplace(new CPU_DoneEvent(time));

				if (CPUAvailable == false)
					readyQueue.insert(currentTask);
				else
				{
					std::cout << "It's time: " << time << " and IO has finished." << std::endl;
					if (!cache->isAvailable(pageNumber))
					{
						if (cache->isFull())
						{
							cache->replace(pageNumber);
							std::cout << "Page fault." << std::endl;
							faults++;
							eventQueue.emplace(new CPU_DoneEvent(time + timePenalty));
							CPUAvailable = false;
						}
						else
						{
							cache->add(pageNumber);
							std::cout << "Page fault." << std::endl;
							eventQueue.emplace(new CPU_DoneEvent(time + timePenalty));
							CPUAvailable = false;
						}
					}
				}

				time += 0.9;
			}
			else
			{
				pageNumber = generatePageNumber(memorySize);
				std::cout << "It's time: " << time << " and a task has begun." << std::endl;
				if (CPUAvailable == false)
					readyQueue.insert(currentTask);
				else
				{
					eventQueue.emplace(new CPU_DoneEvent(time));
					std::cout << "It's time: " << time << " and a new task has begun." << std::endl;
					CPUAvailable = false;
				}

				time += 1.9;
			}
	} while (currentEvent->getTime() != done->getTime());

	std::cout << "It's time: " << time << " and the simulation has finished with " << faults << " page faults." << std::endl;
}
	