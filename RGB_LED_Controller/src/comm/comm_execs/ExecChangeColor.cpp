/* 
* ExecChangeColor.cpp
*
* Created: 6/18/2016 12:02:11 PM
* Author: Alex
*/

#include "ExecChangeColor.h"
#include "../src/hardware_drivers/RGB_Led.h"

using namespace LedCommandExecutors;

// default constructor
ExecChangeColor::ExecChangeColor()
{
} //ExecChangeColor

// default destructor
ExecChangeColor::~ExecChangeColor()
{
} //~ExecChangeColor

bool ExecChangeColor::executeCommand(IncomingCommand* pCommand){
	Color* pColor = (Color*) pCommand->getBufferPtr();
	RGB_Led::setColor(pColor);
	return false;
}

bool ExecChangeColor::revertCommand(IncomingCommand* pCommand){
	
}


