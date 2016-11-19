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
	CompletionStatus baseCompletionStatus;
	//the "queue" / vector of commands that are ready to be run
	//this queue constitutes all the commands entered on one command
	//line at a time
	vector< vector<Command*> > commands;
public:
	//delete all commands in the "queue" (vector)
	//so that there are no memory leaks
	~CommandContainer()
	{
		for (unsigned i = 0; i < commands.size(); i++)
		{
			for (unsigned j = 0; j < commands.at(i).size(); j++)
			{
				delete commands.at(i).at(j);
				commands.at(i).at(j) = 0;
			}
		}
		commands.clear();
	}
	//default constructor
	CommandContainer()
	{
	}
	//add a command to the queue
	void addCommand(vector<Command*> cmds)
	{
		commands.push_back(cmds);
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
			for (unsigned j = 0; j < commands.at(i).size(); j++)
			{
				delete commands.at(i).at(j);
				commands.at(i).at(j) = 0;
			}
		}
		commands.clear();
	}
	void runCommandQueue(int &status)
	{
		for (unsigned j = 0; j < commands.size(); j++)
		{
			if (commands.at(j).at(0)->compStatus == precedence)
			{
				if (baseCompletionStatus == completed && commands.at(j).at(0)->GetCommandName() == "&&")
				{
					j++;
					goto runComms;
				}
				else if (baseCompletionStatus == failed && commands.at(j).at(0)->GetCommandName() == "||")
				{
					j++;
					goto runComms;
				}
				else
				{
					j++;
					goto finishedComms;
				}
			}

			runComms:
			for (unsigned i = 0; i < commands.at(j).size(); i++)
			{
				//if current command has a solo for execution
				//status run it independently
				if (commands.at(j).at(i)->execStatus == solo)
				{
					prevCompletionStatus = commands.at(j).at(i)->RunCommand(status);
					if (status == 1)
						return;
				}
				//if current command has an and for execution 
				//status and prev succeeded then run command
				else if (commands.at(j).at(i)->execStatus == required && prevCompletionStatus == completed)
				{
					prevCompletionStatus = commands.at(j).at(i)->RunCommand(status);
					if (status == 1)
						return;
				}
				//if current command has an or for execution
				//status and prev failed then run command
				else if (commands.at(j).at(i)->execStatus == optional && prevCompletionStatus == failed)
				{
					prevCompletionStatus = commands.at(j).at(i)->RunCommand(status);
					if (status == 1)
						return;
				}
			}
			finishedComms:
			baseCompletionStatus = prevCompletionStatus;
		}
	}
};
#endif