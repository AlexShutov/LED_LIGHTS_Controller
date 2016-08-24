/* 
* EECommandExecutor.cpp
*
* Created: 8/16/2016 2:44:18 AM
* Author: Alex
*/


#include "EECommandExecutor.h"
#include "../src/hardware_drivers/RGB_Led.h"
using namespace EESupport;

// default constructor
EECommandExecutor::EECommandExecutor()
{
} //EECommandExecutor

// default destructor
EECommandExecutor::~EECommandExecutor()
{
} //~EECommandExecutor



void EECommandExecutor::setEEPlayer(EEPlayer* pPlayer)
{
	pEEPlayer = pPlayer;
}

bool EECommandExecutor::isRGBCommand()
{
	return false;
}

bool EECommandExecutor::isCommandResumable()
{
	return false;
}

bool EECommandExecutor::stopCommand(uint8_t commandCode)
{
	return false;
}

bool EECommandExecutor::resumeCommand(uint8_t commandCode)
{
	return false;
}

bool EECommandExecutor::executeCommand(IncomingCommand* pCommand)
{
	// get command header, it comes first in data block
	EECommandData* pCommHeader = 
		(EECommandData*) pCommand->getBufferPtr();
	
	if (pCommHeader->isLoadCommand){
	// tell player to load command from specific cell
		pEEPlayer->loadAndProcessCell(pCommHeader->cellIndex);
		return true;
	}
	// if we here, command is in 'save' mode
	if (pCommHeader->eraseCell){
		// mark cell as unused, actual command data remains
		// intact, only ee header gets overwritten
		pEEPlayer->markCellAsUnused(pCommHeader->cellIndex);
		return true;
	}
	return true;
}

