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
	commandReceiver.setCommandExecutor(&execChain);
	// connect command receiver to uart. if some command executor need to 
	// change data source, we can pass command receiver and uart data source to that
	// executor (for example, restoring and saving current command to eeprom)
	commandReceiver.setSourceOfData(&uart);
	
	// TimedPulse under index 0 generates timing pulses for RGB LED sequence 
	ledLightsSequencePlayer.setPulseGeneratorIndex(0);
	ledLightsSequencePlayer.init();
	
	/* initialize command processors for LED related commands */
	setupLEDExecutors();
	
}

void CommExecutorFacade::setupLEDExecutors(){
	
	execLightSequence.setCommandCode(COMMAND_CODE_LIGHT_SEQUENCE);
	execLightSequence.setSequencePlayer(&ledLightsSequencePlayer);
	execChain.addExecutor(&execLightSequence);
	
	execChangeColor.setCommandCode(COMMAND_CODE_CHANGE_COLOR);
	execChain.addExecutor(&execChangeColor);
	
	
}
