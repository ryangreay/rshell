#ifndef __CMDCONT_H_INCLUDED__ 
#define __CMDCONT_H_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include "Command.h"

using namespace std;
class CommandContainer
{
private:
	//this tells current chained commands whether to run or not
	CompletionStatus prevCompletionStatus;
	//the "queue" / vector of commands that are ready to be run
	//this queue constitutes all the commands entered on one command
	//line at a time
	vector<Command*> commands;
public:
	//delete all commands in the "queue" (vector)
	//so that there are no memory leaks
	~CommandContainer()
	{
		for (unsigned i = 0; i < commands.size(); i++)
		{
			delete commands.at(i);
			commands.at(i) = 0;
		}
		commands.clear();
	}
	//default constructor
	CommandContainer()
	{
	}
	//add a command to the queue
	void addCommand(Command* cmd)
	{
		commands.push_back(cmd);
	}
	//print - for testing purposes only
	//void print()
	//{
	//    for (unsigned i = 0; i < commands.size(); i++)
	//    {
	//        cout << commands.at(i)->command << " " << commands.at(i)->compStatus << " ";
	//        cout << commands.at(i)->execStatus << " " << commands.at(i)->arguments[0] << endl;
	//    }
	//}
	//empty out the queue for a new set a commands
	void clearCommandQueue()
	{
		for (unsigned i = 0; i < commands.size(); i++)
		{
			delete commands.at(i);
			commands.at(i) = 0;
		}
		commands.clear();
	}
	void runCommandQueue(int &status)
	{
		//first command in queue is always solo and can be run independently
		if (commands.size() > 0)
			prevCompletionStatus = commands.at(0)->RunCommand(status);
		for (unsigned i = 1; i < commands.size(); i++)
		{
			//if current command has a solo for execution
			//status run it independently
			if (commands.at(i)->execStatus == solo)
				prevCompletionStatus = commands.at(i)->RunCommand(status);
			//if current command has an and for execution 
			//status and prev succeeded then run command
			else if (commands.at(i)->execStatus == required && prevCompletionStatus == completed)
			{
				prevCompletionStatus = commands.at(i)->RunCommand(status);
			}
			//if current command has an or for execution
			//status and prev failed then run command
			else if (commands.at(i)->execStatus == optional && prevCompletionStatus == failed)
			{
				prevCompletionStatus = commands.at(i)->RunCommand(status);
			}
			if (status == 1)
			    return;
		}
	}
};
#endif