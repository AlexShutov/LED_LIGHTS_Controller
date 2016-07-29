/* 
* CompositeChainExecutor.cpp
*
* Created: 6/18/2016 10:54:31 AM
* Author: Alex
*/


#include "CompositeChainExecutor.h"

// default constructor
CompositeChainExecutor::CompositeChainExecutor()
{
	// set all exec pointers to 0 (we use those values during lookup)
	for (uint8_t i = 0; i < MAX_NUMBER_OF_EXECS; ++i){
		execChain[i] = 0;
	}
	chainLenght = 0;
} //CompositeChainExecutor

// default destructor
CompositeChainExecutor::~CompositeChainExecutor()
{
} //~CompositeChainExecutor

bool CompositeChainExecutor::executeCommand(IncomingCommand* pCommand){
	uint8_t commandCode = pCommand->getCommandCode();
	CommandExecutor* pFound = getExecutor(commandCode);
	if (pFound){
		pFound->executeCommand(pCommand);
		return true;
	}
	return false;
}

bool CompositeChainExecutor::revertCommand(IncomingCommand* pCommand){
	uint8_t commandCode = pCommand->getCommandCode();
	CommandExecutor* pFound = getExecutor(commandCode);
	if (pFound){
		pFound->revertCommand(pCommand);
		return true;
	}
	return false;
}

bool CompositeChainExecutor::addExecutor(CommandExecutor* pExec){
	if (chainLenght >= MAX_NUMBER_OF_EXECS){
		return false;
	}
	execChain[chainLenght++] = pExec;	
	return true;
}

CommandExecutor* CompositeChainExecutor::getExecutor(uint8_t commandCode){
	for (uint8_t i = 0; i < chainLenght; ++i){
		if (execChain[i]->getCommandCode() == commandCode){
			return execChain[i];
		}
	}
	return 0;
}

uint8_t CompositeChainExecutor::getCommandCode(){
	return 255;
}


