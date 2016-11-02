#ifndef __CMD_H_INCLUDED__ 
#define __CMD_H_INCLUDED__
#include <iostream>
#include <string>
#include "unistd.h"
#include "stdio.h"
#include "signal.h"
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
	Command(ExecutionStatus es, CompletionStatus cs, char* cmd, char** args) 
	{
		execStatus = es;
		compStatus = cs;
		command = cmd;
		arguments = args;
	}
	//this runs an individual command at a time
	//It should have a provision for an error using perror
	//It should have a provision for an exit command
	//It should have a provision for cd command
	//It should return the command status - completed or failed
	//It uses fork, wait, and execvp to run commands
	CompletionStatus RunCommand(int &status)
	{
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
				if (arguments[1] == NULL)
					chdir("/");
				else
					chdir(arguments[1]);
				perror(command);
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
				    string temp = ""; temp += command;
					temp += " failed";
					//execute the command provided with command and arguments
					execvp(command, arguments);

					//the execvp failed so we call perror
					perror(temp.c_str());
					compStatus = failed;
					return failed;
				}
				else
				{
					// Wait for the child
					if (waitpid(pid, 0, 0) < 0)
					{
						perror("Error waiting for the child process");
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