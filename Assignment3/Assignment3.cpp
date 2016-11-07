// Brian Bowles, Assignment 21, January , 2015.
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

// Named Constants.
const int MAX_ITERATION = 2000;
const int WIDTH = 800;
const int PIC_SIZE = 800;
const int MAX_THREAD_AMOUNT = 16;
int iterationArray[PIC_SIZE][PIC_SIZE];

// Mandelbrot function creates an image of the mandelbrot set.
void mandelbrot (int begin, int end)
{
	int iteration = 0;
	
	// Set the size and scale of the image.
	for (int i = begin; i < end; i++)
		for (int j = 0; j < WIDTH; j++)
		{
			auto x0 = j * (3.0 / PIC_SIZE) - 2.0;
			auto y0 = i * (2.5 / PIC_SIZE) - 1.25;
			auto x = 0.0;
			auto y = 0.0;

			iteration = 0;

			// Get value for iteration.
			while ((x * x + y * y < 2.0 * 2.0) && iteration < MAX_ITERATION)
			{
				auto xtemp = x * x - y * y + x0;
				y = 2.0 * x * y + y0;
				x = xtemp;
				iteration += 1;
			}
			
			iterationArray[i][j] = iteration;
		}
}

// Time times how long it takes to execute a given function.
template <typename F>
double time(F const &f)
{
	auto start = std::chrono::system_clock::now();
	f();
	auto stop = std::chrono::system_clock::now();
	auto temp = std::chrono::duration_cast<std::chrono::milliseconds > (stop - start).count();
	return temp;
}

// This function generates part of the mandelbrot.
void threadMandel(int number)
{
	std::vector<std::thread> threadpool(number);
	for (int i = 0; i < threadpool.size(); i++)
		threadpool[i] = std::thread([i, number](){mandelbrot(i*PIC_SIZE/number,(i+1)*PIC_SIZE/number);});
	
	for (int i = 0; i < threadpool.size(); i++)
		threadpool[i].join();
}

// Prints the image of the mandelbrot.
void print()
{
	std::ofstream fout("image.ppm");
	fout << "P3" << std::endl;
	fout << PIC_SIZE << " " << PIC_SIZE << std::endl;
	fout << "255" << std::endl;

	// Print the image of the mandelbrot.
	for (int i = 0; i < PIC_SIZE; i++)
		for (int j = 0; j < PIC_SIZE; j++)
		{
			if (iterationArray[i][j] >= MAX_ITERATION)
				fout << "0 0 0 ";
			else
				fout << "0 0 255 ";
		}
}

// Main executes the time and threadMandel functions in parallel 
// and keeps track of performance of mandelbrot according to number of threads.
int main () 
{
	for (int i = 1; i <= MAX_THREAD_AMOUNT; i*=2)
	{
		auto time_ = time([i](){threadMandel(i);});
		std::cout << "The time it took to display the image was with " << i;
		std::cout << " thread(s) is: " << time_ << " ms." << std::endl;
	}

	print();
	return 0;
}