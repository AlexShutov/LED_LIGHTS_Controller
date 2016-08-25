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
	// save command to EEPROM (RGB, or first command)
	// command header comes right after EECommandData in data buffer
	IncomingCommand* pCmd = (IncomingCommand*)(pCommHeader + 1);
	// data section is after command header (Incoming command)
	char* pData = (char*) (pCmd + 1);
	// just for making sure everything is ok
	pCmd->setBufferPtr(pData);
	uint8_t commDataSize = pCmd->getDataBlockSize();
	// left 0 if there is no background command
	IncomingCommand* pSecondCmd = 0;
	char* pSecondData = 0;
	if (pCommHeader->hasBackgroundCommand){
		// get pointer to background command header
		pSecondCmd = (IncomingCommand*)( pData + commDataSize);
		pSecondData = (char*)(pSecondCmd + 1);
		// just to make sure
		pSecondCmd->setBufferPtr(pSecondData);
	}
	// check cell index
	uint8_t cellIndex = pCommHeader->cellIndex;
	if (cellIndex >= NUMBER_OF_MEMORY_CELL){
		cellIndex = NUMBER_OF_MEMORY_CELL - 1;
	}
	// save commands into EEPROM by using EEPlayer
	pEEPlayer->saveToCell(cellIndex, pCommHeader->hasBackgroundCommand,
		pCmd, pSecondCmd);
	return true;
}

