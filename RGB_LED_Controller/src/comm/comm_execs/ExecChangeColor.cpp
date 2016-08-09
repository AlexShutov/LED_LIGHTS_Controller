/* 
* ExecChangeColor.cpp
*
* Created: 6/18/2016 12:02:11 PM
* Author: Alex
*/

#include "ExecChangeColor.h"

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
	currColor = *pColor;
	// presumable color data is copied, process command
	return resumeCommand(pCommand->getCommandCode());
}

bool ExecChangeColor::isRGBCommand()
{
	return true;
}

bool ExecChangeColor::isCommandResumable()
{
	return true;
}

bool ExecChangeColor::stopCommand(uint8_t commandCode)
{
	if (commandCode != getCommandCode())
	{
		return false;
	}
	Color c;
	Color::clear(&c);
	RGB_Led::setColor(&c);
	return true;
}

bool ExecChangeColor::resumeCommand(uint8_t commandCode){
	if (commandCode != getCommandCode()){
		// command code mismatch - some mistake has occurred
		return false;
	}
	RGB_Led::setColor(&currColor);
	return true;
}




