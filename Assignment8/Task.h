// Task Class
#ifndef TASK_H
#define TASK_H
#include <vector>
#include <string>
#include <random>
#include "Burst.h"

class Task
{
	private: 
		double startTime;
		double stopTime;
		std::vector<Burst*> bursts;
		double latency;
		double responseTime;
		double time;
		
	protected:
		double generateStartTime() 
		{
			std::random_device randDevice;
			std::mt19937 mt(randDevice());
			std::normal_distribution<double> norm(15, 2.3);
			return norm(randDevice);
		}
	
	public:
		/*~Task()
		{
			delete this;
		}*/

		int burstsSize()
		{
			return bursts.size();
		}
	
		void setBursts(std::vector<Burst*> b)
		{
			for (int i = 0; i < b.size(); i++)
				bursts.push_back(b[i]);
		}
	
		void deleteBurst() 
		{
			bursts.erase(bursts.begin());
		}
		
		Burst *getBurst()
		{
			if (bursts.size() >= 1)
				return bursts.front();
			else
				return NULL;
		}
	
		void setStartTime()
		{
			startTime = generateStartTime();
		}
	
		double getStartTime()
		{
			return startTime;
		}
	
		void setStopTime(double t)
		{
			stopTime = t;
		}
	
		double getStopTime()
		{
			return stopTime;
		}
	
		double getLatency()
		{
			return latency;
		}
	
		double getResponsTime()
		{
			return responseTime;
		}
};
#endif