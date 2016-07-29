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
	int chainLenght;

//functions
public:
	CompositeChainExecutor();
	~CompositeChainExecutor();
	
	// 'Chain of commands' pattern
	virtual bool executeCommand(IncomingCommand* pCommand);
	virtual bool revertCommand(IncomingCommand* pCommand);
	/* find first empty place in exec chain and insert exec to into it */
	bool addExecutor(CommandExecutor* pExec);
	
	/* find and return executor by command code */
	CommandExecutor* getExecutor(uint8_t commandCode);
	
	uint8_t getCommandCode();
	
protected:
private:
	CompositeChainExecutor( const CompositeChainExecutor &c );
	CompositeChainExecutor& operator=( const CompositeChainExecutor &c );
	
}; //CompositeChainExecutor

#endif //__COMPOSITECHAINEXECUTOR_H__
