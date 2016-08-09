/* 
* CommandExecutor.h
*
* Created: 6/18/2016 10:24:49 AM
* Author: Alex
*/


#ifndef __COMMANDEXECUTOR_H__
#define __COMMANDEXECUTOR_H__

#include <avr/io.h>
#include "../src/timed_pulse/TimeIntervalGenerator.h"
#include "IncomingCommand.h"

class CommandExecutor;

class ExternalEndCallback
{
public:
	virtual void onSequenceRestarted(uint8_t commandCode) = 0;
	virtual void onSequenceEnded(uint8_t commandCode) = 0;	
};

/**
	Adapted version of Decorator. uses external callback and some
	action, defined by command executor. (pEndAction)
*/
class EndCallbackDispatcher : public TimeIntervalGeneration::EventCallback
{
private:
	CommandExecutor* pExec;
	ExternalEndCallback* pCallback;
	TimeIntervalGeneration::EventCallback* pEndAction;
public:
	EndCallbackDispatcher();
	void onPulseStarted();
	void onPulseEnded();
	void setExec(CommandExecutor* pExec);
	void setExternalCallback(ExternalEndCallback* pCallback);
	ExternalEndCallback* getExternalCallback();
	void setEndAction(TimeIntervalGeneration::EventCallback* pAction);
};

class CommandExecutor
{
private:
	uint8_t commandCode;
	EndCallbackDispatcher* pEndCallbackDispatcher;
	
//functions
public:
	virtual ~CommandExecutor();
	
	virtual bool executeCommand(IncomingCommand* pCommand) = 0;
	/*  get command code of supported command. Every executor support only one 
		command type
	*/
	uint8_t getCommandCode();
	
	/**
		Device has two channels- rgb led and strobes channel.
		Strobes can have only one active command- it either active or not.
		On the other hand, there is few commands for colorful diods and some command
		may be running all time (loop mode). Say, we have one command (constant color, 
		for example). And when this command active, device receive another command, say
		'play light sequence one and finish'. After this command finishes, led  will be
		turned off and we need to resume old command (set previous color). We don't need
		stack of commands, exactly, only last command will be enough.
	*/
	virtual bool isRGBCommand() = 0;
	
	/**
		We can resume command if it has been activated in 'loop' mode or if it change
		state just once ('set color').
		This is the reason why every command has its own buffer for data - we can
		easily resume it. Otherwise, we could use few buffers to store command data and
		switch those buffer instead, but it will make app more complex.
	*/
	virtual bool isCommandResumable() = 0;
	
	/**
		stop executing this command and set channel to 'OFF' state.
		Method need 'commandCode' because CompositeExecutor won't know what
		executor to use otherwise, and it allow to make additional check.
	*/
	virtual bool stopCommand(uint8_t commandCode) = 0;
	
	/*
		Resume running command. Executor can do it because each executor has
		buffer, storing command data.
	*/
	virtual bool resumeCommand(uint8_t commandCode) = 0;
	
	void setCommandCode(uint8_t code);

	void setExternalEndCallback(ExternalEndCallback* pCallback);
	ExternalEndCallback* getExternalEndCallback();
	
	TimeIntervalGeneration::EventCallback* getDecoratedEndCallback();

protected:
	/*
		Pointer to EndCallbackDispatcher instance MUST be set if you want to
		use external callback.
	*/
	void setEndCallbackDispatcher(EndCallbackDispatcher* pDispatcher);
	/*
		Set end action in ExternalCallback (dispatcher must be set before 
		calling this method )
	*/
	void setEndAction( TimeIntervalGeneration::EventCallback* pAction);
}; //CommandExecutor

#endif //__COMMANDEXECUTOR_H__
