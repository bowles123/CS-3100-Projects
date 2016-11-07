// Brian Bowles, Assignment 4, 02/04/15.
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <sstream>

const int STD_OUT = 1;
const int STD_IN = 0;

// Function to execute a pipe command.
void executePipe(std::vector<std::string> &executables, std::vector<std::string> &history)
{
	std::vector<std::vector<std::string>> commands;
	double ptime = 0.0;
	int p[2];
	std::string temp;
	pipe(p);
	commands.resize(executables.size()); 

	for (int i = 0; i < commands.size(); i++)
	{
		std::stringstream stream(executables[i]);
		
		while(stream >> temp)
		{
			commands[i].push_back(temp);
		}
	}

	for (int i = 0; i < commands.size(); i++)
	{std::cout << "i: " << i << std::endl;
		if (i == 0)
		{
			//close(p[0]);
			dup2(p[1], STD_OUT);
		}
		else if (i == commands.size() - 1)
		{
			dup2(p[0], STD_IN);
			dup2(STD_OUT, p[1]);
		}
		else
		{
			dup2(p[0], STD_IN);
			// pipe(p);
			dup2(p[1], STD_OUT);
		}

		// Seperate sparsed input into command and arguments.
		const char *command = commands[i][0].c_str();
		char **args = new char*[commands[i].size()];
		
		// Keep track of the history
		history.push_back(command);

		for (int j = 0; j < commands[i].size(); j++)
		{
			args[i] = new char[commands[i][j].size()];
			std::strcpy(args[i], commands[i][j].c_str());
		}

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
			for (int j = 0; j < commands[i].size(); j++)
				std::cout << args[j] << " ";
			std::cout << "not found." << std::endl;
			break;
		}
		// Parent
		else
		{std::cout << "3." << std::endl;
			// Time child processes.
			int status;
			auto start = std::chrono::system_clock::now();
			waitpid(pid, &status, 0);
			close(p[1]);
			auto stop = std::chrono::system_clock::now(); 

			std::chrono::duration<double> time = stop - start; 
			ptime += time.count();

		}
	
		// Deallocate memory.
		for (int i = 0; i < commands[i].size(); i++)
		{
			delete [] args[i];
			args[i] = NULL;
		}
		delete [] args;
		args = NULL;
	}
}

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
		std::stringstream stream1(line);
		std::stringstream stream2(line);
		std::string str;
		std::string temp;
		std::vector<std::string> processes;

		while(getline(stream2, temp, '|'))
		{
			processes.push_back(temp);
		}

		// Parse user input.
		while(stream1 >> str)
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

		if (processes.size() != 1)
			executePipe(processes, history);

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

					std::chrono::duration<double> time = stop - start; 
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
