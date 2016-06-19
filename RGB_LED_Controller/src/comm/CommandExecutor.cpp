/* 
* CommandExecutor.cpp
*
* Created: 6/18/2016 10:24:49 AM
* Author: Alex
*/


#include "CommandExecutor.h"


// virtual destructor
CommandExecutor::~CommandExecutor()
{
	commandCode = 0;
} //~CommandExecutor

void CommandExecutor::setCommandCode(uint8_t code){
	commandCode = code;
}

uint8_t CommandExecutor::getCommandCode(){
	return commandCode;
}
