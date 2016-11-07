// Event class.
#ifndef EVENT_H
#define EVENT_H
#include <string>
#include <chrono>
#include <random>

class Event
{
	private:
		double time;
	
	public:
		Event (double t)
		{
			time = t;
		}

		/*virtual ~Event()
		{
			delete this;
		}*/
		
		void setTime(double t) 
		{
			time = t;
		}
	
		double getTime()
		{
			return time;
		}
	
		double generateTime()
		{
			std::random_device randDevice;
			std::mt19937 mt(randDevice());
			std::normal_distribution<> norm(500, 12.3);
			return norm(mt);
		}
		
		virtual std::string eventType(const Event &object) = 0;
};
#endif