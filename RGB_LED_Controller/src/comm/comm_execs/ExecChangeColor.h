/* 
* ExecChangeColor.h
*
* Created: 6/18/2016 12:02:11 PM
* Author: Alex
*/


#ifndef __EXECCHANGECOLOR_H__
#define __EXECCHANGECOLOR_H__

#include "../src/comm/CommandExecutor.h"

#define COMMAND_CODE_CHANGE_COLOR 0

class ExecChangeColor : public CommandExecutor
{
//variables
public:
protected:
private:

//functions
public:
	ExecChangeColor();
	~ExecChangeColor();
	
	virtual bool executeCommand(IncomingCommand* pCommand);
	virtual uint8_t getCommandCode();
	
protected:
private:
	ExecChangeColor( const ExecChangeColor &c );
	ExecChangeColor& operator=( const ExecChangeColor &c );

}; //ExecChangeColor

#endif //__EXECCHANGECOLOR_H__
