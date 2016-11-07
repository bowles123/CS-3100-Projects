// Brian Bowles, Assignment 4, 01/27/15.
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <sstream>

int main()
{
	// Variables.
	std::vector<std::string> history;
	double ptime = 0.0;
	bool quit = false;

	do
	{
		std::vector<std::string> argVec;
		std::string line;

		// Get input from user.
		std::cout << "Please enter a  command followed by the arguments: "; 
		std::getline(std::cin, line);
		std::stringstream stream(line);
		std::string str;

		// Parse user input.
		while(stream >> str)
			argVec.push_back(str);

		if(argVec.size() == 0)
			continue;

		// Seperate sparsed input into command and arguments.
		const char *command = argVec[0].c_str();
		char **args = new char*[argVec.size()];

		for (int i = 0; i < argVec.size(); i++)
		{
			args[i] = new char[argVec[i].size()];
			std::strcpy(args[i], argVec[i].c_str());
		}

		// Keep track of the history
		history.push_back(command);
		std::string comparison(command);

		// Exit if command is exit.
		if (comparison == "exit")
		{
			quit = true;
			continue;
		}

		// Display the history if commmand is history.
		if (comparison == "history")
		{
			for (int i = 0; i < history.size(); i++)
				std::cout << history[i] << " ";
			std::cout << std::endl;
		}
		else
		{
			// Display time taken executing child processes if command is ptime.
			if (comparison == "ptime")
			{
				std::cout << ptime * 1000 << " ms" << std::endl;

			}
			else
			{

				int pid =  fork();

				if (pid < 0)
				{
					std:: cerr << "could not fork" << std::endl;
				}

				// Child
				if(pid == 0)
				{

					execvp(command, args);
					std::cout << command << " ";
					for (int i = 0; i < argVec.size(); i++)
						std::cout << args[i] << " ";
					std::cout << "not found." << std::endl;
					return 0;
				}

				// Parent
				else
				{
					// Time child processes.
					int status;
					auto start = std::chrono::system_clock::now();
					waitpid(pid, &status, 0);
					auto stop = std::chrono::system_clock::now(); 

					std::chrono::duration<double> time = stop - start; // convert to milliseconds.
					ptime += time.count();

				}
			}
		}
		// Deallocate memory.
		for (int i = 0; i < argVec.size(); i++)
		{
			delete [] args[i];
			args[i] = NULL;
		}
		delete [] args;
		args = NULL;
	}
	while(quit == false);
	return 0;
}
