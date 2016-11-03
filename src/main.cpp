#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>
#include "unistd.h"
#include "CommandContainer.h"
#include "Command.h"
#include "CommandEnums.h"
#include "pwd.h"

using namespace std;

Command* createCommand(char* cmd, ExecutionStatus es)
{
	char* commandName;
	vector<char *> arguments;
	char* argSplit;
	char* argContext;
	string commentCheck;
	string commentArg;
	int i = 0;
	//first arg is always the command
	argSplit = strtok_r(cmd, " ", &argContext);
	commandName = argSplit;
	//pick the initial argument
	//argSplit = strtok_r(NULL, " ", &argContext);
	while (argSplit != NULL)
	{
		//check for a comment
		commentCheck = string(argSplit);
		if (commentCheck.find("#") != string::npos)
		{
			if (commentCheck.length() == 1)
				goto comment;
			else
			{
				if (commentCheck.find("#") == 0)
					goto comment;
				else if (commentCheck.find("#") == commentCheck.length() - 1)
				{
					commentArg = argSplit;
					commentArg = commentArg.substr(0, commentArg.length() - 1);
					char *temp = &commentArg[0];
					arguments.push_back(temp); //minus the # at the end
					if (i == 0)
						commandName = temp;
					goto comment;
				}
			}
		}
		else
		{
			arguments.push_back(argSplit);
			if (i == 0)
				commandName = argSplit;
		}
		argSplit = strtok_r(NULL, " ", &argContext);
		i++;
	}

	//if there is a comment in the args, 
	//program flow should jump to this point
	comment:
	arguments.push_back(NULL);
	//after putting the arguments in a vector
	//we transfer them to a char** so that the
	//executevp method can handle them
	char** argv = new char*[arguments.size()];
	for (unsigned i = 0; i < arguments.size(); i++)
		argv[i] = arguments.at(i);
	//argv[arguments.size()] = NULL;
	Command* command = new Command(es, pending, commandName, argv);
	return command;
}

int main()
{
    //used for when exit command is called so that we can safely exit any loop
    //and deallocate resources in the heap
    int status = 0;
	//instantiate a new CommandContainer
	CommandContainer* cmdContainer = new CommandContainer();
	//get the current running user
	struct passwd* pwd = getpwuid(getuid());
	char* user = pwd->pw_name;
	char hostname[500]; gethostname(hostname, 500);
	//split string
	char* split;
	char* chainSplit1;
	char* chainSplit2;
	char* chainContext1;
	char* chainContext2;
	char* context;
	//full command line
	char line[500];
	//Execution status for each command (&& || ;)
	ExecutionStatus execStatus = solo;
	//Command to be created and added to the container and queued up
	Command* command;

	while (status == 0)
	{
		//read in commands, parsing on #, ||, &&, ;
		//creating as many instances of commands as needed 
		//and pushing into cmdContainer
		
		//give it the ability to print the username and hostname
		cout << "[" << user << "@" << hostname << "]" << "$ ";
		cin.getline(line, 400);
	
		split = strtok_r(line, ";", &context);
		while (split != NULL)
		{
			chainSplit1 = strtok_r(split, "&&", &chainContext1);
			while (chainSplit1 != NULL)
			{
				chainSplit2 = strtok_r(chainSplit1, "||", &chainContext2);
				while (chainSplit2 != NULL)
				{
					command = createCommand(chainSplit2, execStatus);
					cmdContainer->addCommand(command);

					chainSplit2 = strtok_r(NULL, "||", &chainContext2);
					execStatus = optional;
				}
				chainSplit1 = strtok_r(NULL, "&&", &chainContext1);
				execStatus = required;
			}
			
			split = strtok_r(NULL, ";", &context);
			execStatus = solo;
		}		
		//execute all the commands in the queue and if one 
		//of the commands is an exit, status will be updated to 1
		// and the program will exit the while loop
		cmdContainer->runCommandQueue(status);
		cmdContainer->clearCommandQueue();
		//cout << endl;
	}
	delete cmdContainer;
	return 0;
}