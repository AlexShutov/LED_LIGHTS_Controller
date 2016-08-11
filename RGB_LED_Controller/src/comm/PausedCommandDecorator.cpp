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
	
	return pDecoree->executeCommand(pCommand);
}

/************************************************************************/
/*		Inherited from       ExternalEndCallback                        */
/************************************************************************/

void PausedCommandDecorator::onSequenceRestarted(uint8_t commandCode)
{
	
}

void PausedCommandDecorator::onSequenceEnded(uint8_t commandCode)
{
	bool isRgb = pExecStorage->getExecutor(commandCode)->isRGBCommand();
	Color c;
	if (isRgb){
		Color::clear(&c);
		c.green = 255;
	} else {
		Color::clear(&c);
		c.red = 255;
	}
	RGB_Led::setColor(&c);
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