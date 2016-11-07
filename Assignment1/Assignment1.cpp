// Brian Bowles, 01/01/14, Assignment 1 CS 3100.

#include <vector>
#include <numeric>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <utility>

template <typename Container>
typename Container:: value_type average(Container const &c)
{
	return std::accumulate(c.begin(), c.end(), 0) / c.size();
}

template <typename Container>
typename Container::value_type stdDev(Container const &c)
{
	std::vector<int> tempContainer(c.size());
	auto squares = std::transform(c.begin(), c.end(), tempContainer.begin(), [&](Container::value_type a){return a*a; });
	return std::accumulate(tempContainer.begin(), tempContainer.end(), 0) / tempContainer.size();
}

template <typename F>
double time(F const &f)
{
	auto start = std::chrono::system_clock::now();
	f();
	auto stop = std::chrono::system_clock::now();
	auto temp = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
	return temp;
}

template <typename F>
//std::pair<std::chrono::duration <double>, std::chrono::duration <double>> stdDev_aveTime(F const &f, int iterations)
std::pair<double,double> stdDev_aveTime(F const &f, int iterations)
{
	//std::vector < std::chrono::duration<double>> times(iterations);
	std::vector <double> times(iterations);
	std::generate(times.begin(), times.end(), [&](){return time(f); });
	auto ave = average(times);
	auto stdDev_ = stdDev(times);
	return std::make_pair(ave, stdDev_);

}

int main()
{
	int ave, dev, num;
	std::vector<double> vector;

	for (int i = 0; i < 100000; i++)
	{
		//std::cout << "Please enter an integer." << std::endl;
		//std::cin >> num;
		vector.push_back(1);
	}

	std::cout << "The average of the container is: " << average(vector) << std::endl;
	std::cout << "The standard deviation of the container is: " << stdDev(vector) << std::endl;
	std::cout << "The time it takes to run the average function is: " << time([&](){average(vector); }) << std::endl;
	auto time_ = stdDev_aveTime(([&](){ average(vector); }), 1000);
	std::cout << "The average runtime of the function was: " << time_.first << std::endl;
	std::cout << "The standard deviation of the runtime of the function was: " << time_.second << std::endl;
	return 0;
}