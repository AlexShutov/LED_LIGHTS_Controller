/* 
* PausedCommandDecorator.cpp
*
* Created: 8/11/2016 1:51:13 AM
* Author: Alex
*/

#include "../src/hardware_drivers/RGB_Led.h"
#include "PausedCommandDecorator.h"

// default constructor
PausedCommandDecorator::PausedCommandDecorator()
{
	pDecoree = 0;
	currentCommand = COMMAND_CODE_NONE;
	previousCommand = COMMAND_CODE_NONE;
} //PausedCommandDecorator

// default destructor
PausedCommandDecorator::~PausedCommandDecorator()
{
} //~PausedCommandDecorator

/************************************************************************/
/*				Inherited from        CommandExecutor                   */
/************************************************************************/

bool PausedCommandDecorator::isRGBCommand()
{
	return pDecoree ? pDecoree->isRGBCommand() : false;
}

bool PausedCommandDecorator::isCommandResumable()
{
	return pDecoree ? pDecoree->isCommandResumable() : false;
}

bool PausedCommandDecorator::stopCommand(uint8_t commandCode)
{
	return pDecoree ? pDecoree->stopCommand(commandCode) : false;
}

bool PausedCommandDecorator::resumeCommand(uint8_t commandCode)
{
	return pDecoree ? pDecoree->resumeCommand(commandCode) : false;
}

void PausedCommandDecorator::setExecStorage(CompositeChainExecutor* pStorage)
{
	pExecStorage = pStorage;
}

void PausedCommandDecorator::setDecoree(CommandExecutor* pDecoreeExecutor)
{
	pDecoree = pDecoreeExecutor;	
}

bool PausedCommandDecorator::executeCommand(IncomingCommand* pCommand)
{
	if (!pDecoree) return false;
	if (!canProceed(pCommand->getCommandCode())){
		return pDecoree->executeCommand(pCommand);
	}
	bool isIncomingCommandResumable = pExecStorage->getExecutor(
		pCommand->getCommandCode())->isCommandResumable();
	handleIncomingCommand(pCommand->getCommandCode(), isIncomingCommandResumable);
	return pDecoree->executeCommand(pCommand);
}

/************************************************************************/
/*		Inherited from       ExternalEndCallback                        */
/************************************************************************/

void PausedCommandDecorator::onSequenceRestarted(uint8_t commandCode)
{
	// do nothing, command not ended.
}

void PausedCommandDecorator::onSequenceEnded(uint8_t commandCode)
{
	handleEndedCommand(commandCode);
}

/************************************************************************/
/*		Logic- specific methods                                         */
/************************************************************************/

bool PausedCommandDecorator::canProceed(uint8_t commandCode)
{
	// command storage isn't set, proceed without history support
	if (!pExecStorage) return false;
	CommandExecutor* pExec = pExecStorage->getExecutor(commandCode);
}

void PausedCommandDecorator::handleIncomingCommand(uint8_t incomingCode, 
												   bool isResumable)
{
	/* This case is when this is very first command */
	if (currentCommand == COMMAND_CODE_NONE){
		// save command as current one
		currentCommand = incomingCode;
		return;
	}
	CommandExecutor* pCurrExec = pExecStorage->getExecutor(currentCommand);
	/* We're were asked to start the same command again - do nothing 
	 here, executor will restart itself
	 */
	if (incomingCode == currentCommand){
		return;
	}
	/* stop active command */
	pCurrExec->stopCommand(currentCommand);
	if (pCurrExec->isCommandResumable()){
		previousCommand = currentCommand;
	}
	currentCommand = incomingCode;
	
}

void PausedCommandDecorator::handleEndedCommand(uint8_t commandCode)
{
	
}