/* 
* EECommandExecutor.cpp
*
* Created: 8/16/2016 2:44:18 AM
* Author: Alex
*/


#include "EECommandExecutor.h"

// default constructor
EECommandExecutor::EECommandExecutor()
{
} //EECommandExecutor

// default destructor
EECommandExecutor::~EECommandExecutor()
{
} //~EECommandExecutor



void EECommandExecutor::setEEPlayer(EESupport::EEPlayer* pPlayer)
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
	return true;
}

