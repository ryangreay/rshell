#ifndef __CMD_H_INCLUDED__ 
#define __CMD_H_INCLUDED__
#include <iostream>
#include <string>
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "ctype.h"
#include "CommandEnums.h"
using namespace std;

class Command
{
private:
    //the command name
	char* command;
	//the arguments associated with a command name
	char** arguments;
	//the number arguments present
	int argumentSize;
public:
	//the current status of a command. This changes after
	//RunCommand() is called in the runcommandqueue and is 
	//used to determine whether chained commands should run in the queue
	CompletionStatus compStatus;
	//this needs to be accessed when the commandqueue runs
	//to determine chaining so it is public
	ExecutionStatus execStatus;
	//destructor - this needs to deallocate
	//arguments and maybe command
	~Command()
	{
	    delete [] arguments;
	}
	//constructor
	Command(ExecutionStatus es, CompletionStatus cs, char* cmd, char** args, int size) 
	{
		execStatus = es;
		compStatus = cs;
		command = cmd;
		arguments = args;
		argumentSize = size;
	}
	string GetCommandName()
	{
		return string(command);
	}
	//this runs an individual command at a time
	//It should have a provision for an error using perror
	//It should have a provision for an exit command
	//It should have a provision for the test command
	//It should have a provision for cd command
	//It should return the command status - completed or failed
	//It uses fork, wait, and execvp to run commands
	CompletionStatus RunCommand(int &status)
	{
		int cStatus = 0;
		int process;
	    //first check for an exit command to safely return
	    //back to main and deallocate any data in the heap
		if (!strcmp(command, "exit"))
	    {
	        status = 1;
	        compStatus = completed;
		    return completed;
	    }
		else
		{
		    //cd command works a little differently and needs to be
		    //handled by itself
			if (!strcmp(command, "cd"))
			{
				int retVal;
				char* currDir = getenv("PWD");
				char* prevDir = getenv("OLDPWD");
				//go to home directory
				if (arguments[1] == NULL)
					retVal = chdir(getenv("HOME"));
				else if (string(arguments[1]) == "-")
					retVal = chdir(prevDir);
				else
					retVal = chdir(arguments[1]);
					
				if (retVal == -1)
				{
					perror(command);
					compStatus = failed;
					return failed;
				}
				else
				{
					if (arguments[1] == NULL)
					{
						setenv("OLDPWD", currDir, 1);
						setenv("PWD", getenv("HOME"), 1);
					}
					else if (string(arguments[1]) == "-")
					{
						setenv("OLDPWD", currDir, 1);
						setenv("PWD", prevDir, 1);
					}
					else
					{
						setenv("OLDPWD", currDir, 1);
						setenv("PWD", arguments[1], 1);
					}
				}
			}
			else if (!strcmp(command, "test"))
			{
				struct stat info;
				bool success = false;
				if (arguments[1] == NULL) //no path and no flag
					success = false;
				else if (arguments[2] == NULL && string(arguments[1]) != "-e" &&
					string(arguments[1]) != "-d" && string(arguments[1]) != "-f") //no flag
				{
					//check only if the directory/file exists 
					if (lstat(arguments[1], &info) != 0)
						success = false;
					else
						success = true;
				}
				else if (arguments[3] == NULL)
				{
					if (lstat(arguments[2], &info) == 0 && 
						string(arguments[1]) == "-e")
						success = true;
					else if (lstat(arguments[2], &info) == 0 && 
						(S_ISDIR(info.st_mode)) &&
						string(arguments[1]) == "-d")
						success = true;
					else if (lstat(arguments[2], &info) == 0 && 
						(S_ISREG(info.st_mode)) &&
						string(arguments[1]) == "-f")
						success = true;
					else
						success = false;
				}
				
				if (success)
				{
					cout << "(True)" << endl;
					compStatus = completed;
					return completed;
				}
				else
				{
					cout << "(False)" << endl;
					compStatus = failed;
					return failed;
				}
			}
			else
			{
			    //first we fork
				pid_t pid = fork();
                //if the fork fails we notfiy the user with perror
				if (pid < 0)
				{
					perror("Error during fork process");
					compStatus = failed;
					return failed;
				}
				else if (pid == 0)
				{
					//execute the command provided with command and arguments
					execvp(command, arguments);

					//the execvp failed so we call perror
					perror(command);
					compStatus = failed;
					return failed;
				}
				else
				{
					// wait on child with waitpid and use cStatus to
					//determin whether the command succeeded
					process = waitpid(pid, &cStatus, 0);
					if (process < 0)
					{
						perror("Error waiting for the child process");
						compStatus = failed;
						return failed;
					}
					if (cStatus > 0)
					{
						compStatus = failed;
						return failed;
					}
				}
			}
		}
		compStatus = completed;
		return completed;
	}
};
#endif