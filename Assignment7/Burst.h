// Burst abstract class.
#ifndef BURST_H
#define BURST_H
#include <string>
#include <random>

class Burst
{
	private: 
		double time;
		double runTime;
	
	public: 
		Burst(double t)
		{
			time = t;
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
			runTime = num;
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