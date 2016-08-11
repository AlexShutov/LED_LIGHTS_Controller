/* 
* PausedCommandDecorator.h
*
* Created: 8/11/2016 1:51:13 AM
* Author: Alex
*/


#ifndef __PAUSEDCOMMANDDECORATOR_H__
#define __PAUSEDCOMMANDDECORATOR_H__

#include "../src/comm/CommandExecutor.h"
#include "../src/comm/comm_execs/CompositeChainExecutor.h"


class PausedCommandDecorator : public CommandExecutor, public ExternalEndCallback
{
//variables
public:
protected:
private:
	/* We need history of foreground (rgb) and background commands. One way to do that- 
	to use 'composite pattern', but here we'll use two nested decoartors - one for 
	foreground and the other - for background command executor.
	Usually this decorator use 'pExecStorage' as decoree also, but here it set 
	explicitly for obiding pattern structure
	*/
	CommandExecutor* pDecoree;
	// keep all command executors and can return executor by command code
	CompositeChainExecutor* pExecStorage;
//functions
public:
	
	/************************************************************************/
	/*	Inherited from     CommandExecutor                                  */
	/************************************************************************/
	
	// those methods are useless here, those just dispatch 
	// call to decoree instance
	virtual bool isRGBCommand();
	virtual bool isCommandResumable();
	virtual bool stopCommand(uint8_t commandCode);
	virtual bool resumeCommand(uint8_t commandCode);
	// the only useful method
	virtual bool executeCommand(IncomingCommand* pCommand);
	
	/************************************************************************/
	/* Inherited from      ExternalEndCallback                              */
	/************************************************************************/
	
	virtual void onSequenceRestarted(uint8_t commandCode);
	virtual void onSequenceEnded(uint8_t commandCode);
	
	void setExecStorage(CompositeChainExecutor* pStorage);
	void setDecoree(CommandExecutor* pDecoreeExecutor);
	PausedCommandDecorator();
	~PausedCommandDecorator();
protected:
private:
	PausedCommandDecorator( const PausedCommandDecorator &c );
	PausedCommandDecorator& operator=( const PausedCommandDecorator &c );
	
	/* check if command history decorator is ready and can process a
	particular command (or dispatch call to decoree otherwise )
	This method check if executor for this command is valid also.
	If returns true - include command into history, false - ignore command
	(but dispatch to decoree executor ).
	Command code is minimal info for checking it.
	*/
	bool canProceed(uint8_t commandCode);

}; //PausedCommandDecorator

#endif //__PAUSEDCOMMANDDECORATOR_H__
