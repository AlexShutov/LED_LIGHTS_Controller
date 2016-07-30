/* 
* CommExecutorFacade.cpp
*
* Created: 6/18/2016 7:33:04 PM
* Author: Alex
*/


#include "CommExecutorFacade.h"

// default constructor
CommExecutorFacade::CommExecutorFacade()
{
} //CommExecutorFacade

// default destructor
CommExecutorFacade::~CommExecutorFacade()
{
} //~CommExecutorFacade

void CommExecutorFacade::pollForCommand(){
	commandReceiver.receiveCommand();
}

void CommExecutorFacade::initialize(){
	// bind command receiver to chain of commands
	commandReceiver.setCommandExecutor(&commChangeColor);
	// connect command receiver to uart. if some command executor need to 
	// change data source, we can pass command receiver and uart data source to that
	// executor (for example, restoring and savong currrent command to eeprom)
	commandReceiver.setSourceOfData(&uart);
	/* initialize command processors for LED related commands */
	setupLEDExecutors();
	
}

void CommExecutorFacade::setupLEDExecutors(){
	commChangeColor.setCommandCode(COMMAND_CODE_CHANGE_COLOR);
	execChain.addExecutor(&commChangeColor);
	
	commLightSequence.setCommandCode(COMMAND_CODE_LIGHT_SEQUENCE);
	execChain.addExecutor(&commLightSequence);
}
