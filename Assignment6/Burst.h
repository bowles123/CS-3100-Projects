// Burst abstract class.
#ifndef BURST_H
#define BURST_H
#include <string>
#include <random>

class Burst
{
	private: 
		double time;
	
	public: 
		Burst(double t)
		{
			time = t;
		}

		/*virtual ~Burst()
		{
			delete this;
		}*/

		virtual double generateTime() = 0;
	
		void setTime() 
		{
			time = generateTime();
		}
	
		double getTime()
		{
			return time;
		}	
};
#endif