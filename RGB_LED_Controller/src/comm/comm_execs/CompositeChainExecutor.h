/* 
* CompositeChainExecutor.h
*
* Created: 6/18/2016 10:54:31 AM
* Author: Alex
*/


#ifndef __COMPOSITECHAINEXECUTOR_H__
#define __COMPOSITECHAINEXECUTOR_H__

#include "../src/comm/CommandExecutor.h"

#define MAX_NUMBER_OF_EXECS 10

class CompositeChainExecutor : public CommandExecutor
{
//variables
public:
protected:
private:

	CommandExecutor* execChain[MAX_NUMBER_OF_EXECS];
	uint8_t chainLenght;

//functions
public:
	CompositeChainExecutor();
	~CompositeChainExecutor();
	
	// 'Chain of commands' pattern
	virtual bool executeCommand(IncomingCommand* pCommand);
	
	/* find first empty place in exec chain and insert exec to into it */
	bool addExecutor(CommandExecutor* pExec);
	
	/* find and return executor by command code */
	CommandExecutor* getExecutor(uint8_t commandCode);
	
	uint8_t getCommandCode();
	/* nope for both */
	bool isRGBCommand();
	bool isCommandResumable();
	bool stopCommand(uint8_t commandCode);
	bool resumeCommand(uint8_t commandCode);
	
	uint8_t getNumberOfChildren();
	CommandExecutor* getExecutorByAddingOrder(uint8_t index);
	
protected:
private:
	CompositeChainExecutor( const CompositeChainExecutor &c );
	CompositeChainExecutor& operator=( const CompositeChainExecutor &c );
	
}; //CompositeChainExecutor

#endif //__COMPOSITECHAINEXECUTOR_H__
