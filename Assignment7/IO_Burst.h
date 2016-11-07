// IO_Burst class.
#ifndef IO_BURST_H
#define IO_BURST_H
#include<string>
#include "Burst.h"

class IO_Burst: public Burst
{
	private:
		int deviceNum;
	
		double generateTime()
		{
			std::random_device randDevice;
			std::mt19937 mt(randDevice());
			std::normal_distribution<> norm(3, 0.5);
			return norm(mt);
		}
	
	public:
		IO_Burst(double time) : Burst(time) {}

		/*virtual ~IO_Burst()
		{
			delete this;
		}*/
	
		void setDeviceNum(int num)
		{
			deviceNum = num;
		}
	
		int getDeviceNum()
		{
			return deviceNum;
		}
};
#endif