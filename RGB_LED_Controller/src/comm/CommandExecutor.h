/* 
* CommandExecutor.h
*
* Created: 6/18/2016 10:24:49 AM
* Author: Alex
*/


#ifndef __COMMANDEXECUTOR_H__
#define __COMMANDEXECUTOR_H__

#include <avr/io.h>
#include "IncomingCommand.h"

class CommandExecutor
{
private:
	uint8_t commandCode;
	
//functions
public:
	virtual ~CommandExecutor();
	
	virtual bool executeCommand(IncomingCommand* pCommand) = 0;
	/*  get command code of supported command. Every executor support only one 
		command type
	*/
	
	virtual bool revertCommand(IncomingCommand* pCommand) = 0;
	uint8_t getCommandCode();
	void setCommandCode(uint8_t code);
	
}; //CommandExecutor

#endif //__COMMANDEXECUTOR_H__
