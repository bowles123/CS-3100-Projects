// Brian Bowles, Assignment 2, January 13, 2015.
#include <fstream>
#include <iostream>
#include <chrono>

// Named Constants.
const int MAX_ITERATION = 2000;
const float STEP_SIZE = 100.00;
const float MAX_SIZE = 1100.00;

// Mandelbrot function creates an image of the mandelbrot set.
void mandelbrot (int pic_size)
{
	int iteration = 0;

	std::ofstream fout("image.ppm");
	fout << "P3" << std::endl;
	fout << pic_size << " " << pic_size << std::endl;
	fout << "255" << std::endl;
	
	// Set the size and scale of the image.
	for (int i = 0; i < pic_size; i++)
		for (int j = 0; j < pic_size; j++)
		{
			auto x0 = j * (3.0 / pic_size) - 2.0;
			auto y0 = i * (2.5 / pic_size) - 1.25;
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

			// Print the image of the mandelbrot.
			if (iteration >= MAX_ITERATION)
				fout << "255 0 0 ";
			else
				fout << "0 0 255 ";
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

// Main executes the time and mandelbrot functions in parallel 
// and keeps track of performance of mandelbrot according to image size.
int main () 
{
	auto time_ = time([](){mandelbrot(STEP_SIZE);});
	std::cout << "The time it took to display the image was: " << time_ << " milliseconds." << std::endl;

	// Track performance of 10 different image sizes.
	for (int i = 200.00; i < MAX_SIZE; i += STEP_SIZE)
	{
		time_ = time([&i](){mandelbrot(i);});
		std::cout << "The time it took to display the image was: " << time_ << " milliseconds." << std::endl;	
	}
	return 0;
}