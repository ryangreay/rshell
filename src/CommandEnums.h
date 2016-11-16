#ifndef __CMDENUM_H_INCLUDED__ 
#define __CMDENUM_H_INCLUDED__
//solo: run a command by itself
//required: run command if previous succeeded
//optional: run command if previous failed
enum ExecutionStatus
{
	solo, required, optional
};
enum CompletionStatus
{
	pending, failed, completed, precedence
};
#endif
