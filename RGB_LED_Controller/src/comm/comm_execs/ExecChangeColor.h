/* 
* ExecChangeColor.h
*
* Created: 6/18/2016 12:02:11 PM
* Author: Alex
*/


#ifndef __EXECCHANGECOLOR_H__
#define __EXECCHANGECOLOR_H__

#include "../src/hardware_drivers/RGB_Led.h"
#include "../src/comm/CommandExecutor.h"


namespace LedCommandExecutors {

#define COMMAND_CODE_CHANGE_COLOR 0

class ExecChangeColor : public CommandExecutor
{
//variables
public:
protected:
private:
	Color currColor;
//functions
public:
	ExecChangeColor();
	virtual ~ExecChangeColor();
	
	virtual bool executeCommand(IncomingCommand* pCommand);
	
	virtual bool isRGBCommand();
	virtual bool isCommandResumable();
	
	virtual bool stopCommand(uint8_t commandCode);
	virtual bool resumeCommand(uint8_t commandCode);
	
protected:
private:
	ExecChangeColor( const ExecChangeColor &c );
	ExecChangeColor& operator=( const ExecChangeColor &c );

}; //ExecChangeColor

}
#endif //__EXECCHANGECOLOR_H__
