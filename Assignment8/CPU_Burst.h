// CPU_Burst class.
#ifndef CPU_BURST_H
#define CPU_BURST_H
#include "Burst.h"
class CPU_Burst: public Burst
{
	private: 
		double generateTime()
		{
			std::random_device randDevice;
			std::mt19937 mt(randDevice());
			std::normal_distribution<double> norm(3, 0.5);
			return norm(mt);
		}
	
	public:
		CPU_Burst(double time) : Burst(time) {}

		/*virtual ~CPU_Burst()
		{
			delete this;
		}*/
};
#endif