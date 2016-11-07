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

class Compare 
{
	public:
		bool operator()(Event* time1, Event* time2)
		{
			if (time1->getTime() < time2->getTime())
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
		std::normal_distribution<double> norm(5000, 6.7);
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

int main () 
{
	// Variables and class objects.
	int numCPUs, numIO_Devices, contextCost, numCPUsAvailable, requestedDevice, currentTasksBurstsSize;
	int IOsAvailable, currentOrigSize, origTasksSize, currentTasksSize, total, available, counter = 1;
	std::vector<int> origSizes;
	std::vector<double> throughputs;
	std::vector<double>latencies;
	std::vector<double> responseTimes;
	std::vector<double> utilizations;
	double taskMix, jobFrequency, doneTime;
	double meanLatency, maxLatency, minLatency, stdDevLatency;
	double meanThroughput, maxThroughput, minThroughput, stdDevThroughput;
	double meanResponseTime, maxResponseTime, minResponseTime, stdDevResponseTime;
	double meanUtilization, maxUtilization, minUtilization, stdDevUtilization;
	std::vector<Task*> tasks = listGenerate(generateNumBursts());
	std::priority_queue<Event*, std::vector<Event*>, Compare> eventQueue;
	std::vector<WaitQueue*> waits;
	FIFO_Queue readyQueue;
	Event *currentEvent = new CPU_DoneEvent(1.0);
	Task *currentTask;
	Burst *currentBurst;
	SimulationDoneEvent *done;
	
	// Get user input for the variables they can change.
	std::cout << "Please enter the number of CPU devices: ";
	std::cin >> numCPUs;
	std::cout << "Please enter the number of IO devices: ";
	std::cin >> numIO_Devices;
	std::cout << "Please enter the percent of tasks that will be CPU bound: ";
	std::cin >> taskMix;
	std::cout << "Please enter how often new tasks will be added to the queue: ";
	std::cin >> jobFrequency;
	std::cout << "Please enter the cost of the context switch: ";
	std::cin >> contextCost;
	
	// Create waitQueue for every IO device.
	for (int i = 0; i < numIO_Devices; i++)
	{
		waits.push_back(new WaitQueue(i, true));
	}
	
	numCPUsAvailable = numCPUs;
	IOsAvailable = numIO_Devices;
	origTasksSize = tasks.size();
	done = new SimulationDoneEvent(generateEndTime());
	doneTime = done->getTime();
	eventQueue.emplace(done);
	currentTasksSize = tasks.size();

	for (int i = 0; i < tasks.size(); i++)
		origSizes.push_back(tasks[0]->burstsSize());
	
	// Push every task onto the event queue.
	for (int i = 0; i < tasks.size(); i++)
		eventQueue.emplace(new NewTask(tasks[i]));
	
	// While the event queue is not empty and the simulation isn't done execute events.	
	while(eventQueue.empty() == false && eventQueue.top()->getTime() != doneTime)
	{
		if (counter == 1)
		{
			eventQueue.pop();
			counter++;
		}

		currentEvent = eventQueue.top();
		currentTask = tasks.front();
		currentBurst = currentTask->getBurst();
		currentTasksBurstsSize = currentTask->burstsSize();
		currentTasksSize = tasks.size();
		eventQueue.pop();
		total = numIO_Devices + numCPUs;
		available = numCPUsAvailable + IOsAvailable;
		utilizations.push_back(available/total);
		
		std::cout << "It is time: " << currentEvent->getTime() << " and";
		
		// If event is new task start the task if CPU is available
		if (currentEvent->eventType(*currentEvent) == "NewTaskEvent")
		{
			int taskNum = (origTasksSize - currentTasksSize) + 1;
			tasks.erase(tasks.begin());
			
			if (tasks.size() == 0)
				latencies.push_back(currentEvent->getTime() - currentTask->getStartTime());
			
			if (numCPUsAvailable != 0)
			{
				std::cout << "task " << taskNum << " has begun." << std::endl;
				eventQueue.emplace(new CPU_DoneEvent(currentEvent->getTime() + contextCost + currentTask->getStartTime()));
				numCPUsAvailable--;
				if (readyQueue.getQueue().size() != 0 && numCPUsAvailable != 0)
				{
					currentTask = tasks.front();
					tasks.erase(tasks.begin());
					eventQueue.emplace(new CPU_DoneEvent(currentEvent->getTime() + contextCost + currentTask->getStartTime()));
					numCPUsAvailable--;
				}
				else
					continue;
			}
			else 
				readyQueue.addTask(currentTask);
			continue;	
		}
		
		//  If event is CPU done task then create a new IO done event if it's device is available.
		if (currentEvent->eventType(*currentEvent) == "CPU_DoneEvent")
		{
			numCPUs++;
			requestedDevice = ((IO_Burst*)currentTask->getBurst())->getDeviceNum();
		
			if (waits[requestedDevice]->isAvailable())
			{
				std::cout << "CPU burst " << currentTasksBurstsSize << " has finished." << std::endl;
				eventQueue.emplace(new IO_DoneEvent(currentEvent->getTime() + currentTask->getBurst()->getTime()));
				IOsAvailable--;
				waits[requestedDevice]->setAvail(false);

				if (readyQueue.getQueue().size() != 0 && numCPUsAvailable != 0)
				{
					currentTask = tasks.front();
					tasks.erase(tasks.begin());
					eventQueue.emplace(new CPU_DoneEvent(currentEvent->getTime() + contextCost + currentTask->getStartTime()));
					numCPUsAvailable--;
				}
				else
				{
					currentTask->deleteBurst();
					continue;
				}
			}
			else
				waits[requestedDevice]->push(currentTask);
			currentTask->deleteBurst();
			continue;
		}
		
		if (currentEvent->eventType(*currentEvent) == "IO_DoneEvent")
		{
			IOsAvailable++;
			waits[((IO_Burst*)currentTask->getBurst())->getDeviceNum()]->setAvail(true);
			if (currentTask->burstsSize() == currentTasksBurstsSize)
				responseTimes.push_back(currentEvent->getTime() - currentTask->getStartTime());
		
			if (numCPUsAvailable != 0)
			{
				std::cout << "IO burst " << currentTasksBurstsSize << " has finished." << std::endl;
				eventQueue.emplace(new CPU_DoneEvent(currentEvent->getTime() + contextCost + currentTask->getStartTime()));
				numCPUsAvailable--;
				
				if (readyQueue.getQueue().size() != 0 && numCPUsAvailable != 0)
				{
					currentTask = tasks.front();
					tasks.erase(tasks.begin());
					eventQueue.emplace(new CPU_DoneEvent(currentEvent->getTime() + contextCost + currentTask->getStartTime()));
					numCPUsAvailable--;
				}
				else
				{
					currentTask->deleteBurst();
					continue;
				}	
			}
			else 
				readyQueue.addTask(currentTask);
			currentTask->deleteBurst();
			continue;
		}
		
		if (currentEvent->eventType(*currentEvent) == "SimulationDoneEvent")
		{
			std:: cout << "the simulation has now finished." << std:: endl;
			break;
		}
	}
	
	// This may have to be changed.
	throughputs.push_back((origTasksSize - currentTasksSize)/currentEvent->getTime());
	/*
	meanResponseTime = average(responseTimes);
	meanLatency = average(latencies);
	meanUtilization = average(utilizations);
	stdDevResponseTime = stdDev(responseTimes);
	stdDevLatency = stdDev(latencies);
	stdDevUtilization = stdDev(utilizations);
	minResponseTime = *std::min_element(responseTimes.begin(), responseTimes.end());
	minLatency = *std::min_element(latencies.begin(), latencies.end());
	minUtilization = *std::min_element(utilizations.begin(), utilizations.end());
	maxResponseTime = *std::max_element(responseTimes.begin(), responseTimes.end());
	maxLatency = *std::max_element(latencies.begin(), latencies.end());
	maxUtilization = *std::max_element(utilizations.begin(), utilizations.end());
		
	std::cout << "Response Times - " << std::endl;
	std::cout << "\tAverage: " << meanResponseTime << std::endl;
	std::cout << "\tStandard deviation: " << stdDevResponseTime << std::endl;
	std::cout << "\tMinimum: " << minResponseTime << std::endl;
	std::cout << "\tMaximum: " << maxResponseTime << std::endl;
	
	std::cout << "Latencies - " << std::endl;
	std::cout << "\tAverage: " << meanLatency << std::endl;
	std::cout << "\tStandard deviation: " << stdDevLatency << std::endl;
	std::cout << "\tMinimum: " << minLatency << std::endl;
	std::cout << "\tMaximum: " << maxLatency << std::endl;
	
	std::cout << "Utilization - " << std::endl;
	std::cout << "\tAverage: " << meanUtilization << std::endl;
	std::cout << "\tStandard deviation: " << stdDevUtilization << std::endl;
	std::cout << "\tMinimum: " << minUtilization << std::endl;
	std::cout << "\tMaximum: " << maxUtilization << std::endl; */

	/*meanThroughput = average(throughputs);
	stdDevThroughput = stdDev(throughputs);
	minThroughput = *std::min_element(throughputs.begin(), throughputs.end());
	maxThroughput = *std::max_element(throughputs.begin(), throughputs.end()); 

	std::cout << "Throughputs - " << std::endl;
	std::cout << "\tAverage: " << meanThroughput << std::endl;
	std::cout << "\tStandard deviation: " << stdDevThroughput << std::endl;
	std::cout << "\tMinimum: " << minThroughput << std::endl;
	std::cout << "\tMaximum: " << maxThroughput << std::endl;*/
}