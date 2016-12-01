#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <stdio.h>
#include "stdlib.h"
#include "unistd.h"
#include "CommandContainer.h"
#include "Command.h"
#include "CommandEnums.h"
#include "pwd.h"

using namespace std;

void createCommand(vector<Command*>& commandVec, char* cmd, ExecutionStatus es)
{
	char* commandName;
	vector<char *> arguments;
	char* argSplit;
	char* argContext;
	string val;
	string commentArg;
	int i = 0;
	CompletionStatus cs = pending;
	//check if it is the symbolic version of the test command, if so 
	//change the first '[' to test and remove ']' or replace with ''
	string comm = string(cmd);
	if (comm.find("[") != string::npos)
	{
		comm.erase(comm.find("["), 1);
		comm.erase(comm.find("]"), 1);
		comm = "test " + comm;
	}
	char *current = new char[comm.length() + 1]; // or
	strcpy(current, comm.c_str());
	//first arg is always the command
	argSplit = strtok_r(current, " ", &argContext);
	commandName = argSplit;
	//pick the initial argument
	//argSplit = strtok_r(NULL, " ", &argContext);
	while (argSplit != NULL)
	{
		val = string(argSplit); 
		if (val == "&&" || val == "||")
		{
			cs = precedence;
			goto comment;
		}
		else
		{
			//check for a comment
			if (val.find("#") != string::npos)
			{
				if (val.length() == 1)
					goto comment;
				else
				{
					if (val.find("#") == 0)
						goto comment;
					else if (val.find("#") == val.length() - 1)
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
	Command* command = new Command(es, cs, commandName, argv, arguments.size());
	commandVec.push_back(command);
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
	string output = string(user) + "@" + string(hostname);
	string currDir = "";
	//split string
	bool andCheck = false;
	bool orCheck = false;
	bool success = true;
	string val;
	char* split;
	char* parenSplit;
	char* chainSplit1;
	char* chainSplit2;
	char* parenSplitContext;
	char* chainContext1;
	char* chainContext2;
	char* context;
	//full command line
	char line[500];
	//Execution status for each command (&& || ;)
	ExecutionStatus execStatus = solo;
	//Command to be created and added to the container and queued up
	vector<Command*> command = vector<Command*>();
	string andString = "&&"; string orString = "||";

	while (status == 0)
	{
		//read in commands, parsing on #, ||, &&, ;
		//creating as many instances of commands as needed 
		//and pushing into cmdContainer
		
		//give it the ability to print the username and hostname
		currDir = string(getenv("PWD"));//.substr(string(getenv("PWD")).find_last_of("/") + 1);
		cout << "[" << output << " " << currDir << "]" << "$ ";
		cin.getline(line, 400);
	
		split = strtok_r(line, ";", &context);
		while (split != NULL)
		{
			parenSplit = strtok_r(split, "()", &parenSplitContext);
			while (parenSplit != NULL)
			{
				andCheck = false; orCheck = false; val = string(parenSplit);
				if (val == "&&" || val == " && " || val == "&& " || val == " &&")
				{
					andCheck = true;
					goto create;
				}
				else if (val == "||" || val == " || " || val == "|| " || val == " ||")
				{
					orCheck = true;
					goto create;
				}
				chainSplit1 = strtok_r(parenSplit, "&&", &chainContext1);
				while (chainSplit1 != NULL)
				{
					chainSplit2 = strtok_r(chainSplit1, "||", &chainContext2);
					while (chainSplit2 != NULL)
					{
						create:
						if (andCheck) { chainSplit2 = new char[3]; strcpy(chainSplit2, andString.c_str()); }
						else if (orCheck) { chainSplit2 = new char[3]; strcpy(chainSplit2, orString.c_str()); }

						if (string(chainSplit2) != " ")
						{
							createCommand(command, chainSplit2, execStatus);
							success = true;
						}
						else
							success = false;

						if (andCheck || orCheck)
							goto newParen;
						chainSplit2 = strtok_r(NULL, "||", &chainContext2);
						execStatus = optional;
					}
					chainSplit1 = strtok_r(NULL, "&&", &chainContext1);
					execStatus = required;
				}
				newParen:
				if (success)
					cmdContainer->addCommand(command);
				command = vector<Command*>();
				parenSplit = strtok_r(NULL, "()", &parenSplitContext);
				execStatus = solo;
			}

			split = strtok_r(NULL, ";", &context);
			execStatus = solo;
		}
		//execute all the commands in the queue and if one 
		//of the commands is an exit, status will be updated to 1
		// and the program will exit the while loop
		cmdContainer->runCommandQueue(status);
		cmdContainer->clearCommandQueue();
	}
	delete cmdContainer;
	return 0;
}
