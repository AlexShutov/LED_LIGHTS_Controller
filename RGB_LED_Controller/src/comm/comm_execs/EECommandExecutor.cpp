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
	Color c;
	c.red = 200;
	c.green=  40;
	c.blue = 255;
	RGB_Led::setColor(&c);
	return true;
}

