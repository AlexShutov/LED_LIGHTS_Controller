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
	// initialize strobe channel
	initStrobeChannel();
	// bind command receiver to chain of commands
	commandReceiver.setCommandExecutor(&execChain);
	// connect command receiver to uart. if some command executor need to 
	// change data source, we can pass command receiver and uart data source to that
	// executor (for example, restoring and saving current command to eeprom)
	commandReceiver.setSourceOfData(&uart);
	
	// TimedPulse under index 0 generates timing pulses for RGB LED sequence 
	ledLightsSequencePlayer.setPulseGeneratorIndex(0);
	ledLightsSequencePlayer.init();
	
	// TimedPulse with index 1 - handles strobe flashes
	strobePlayer.setPulseGeneratorIndex(1);
	strobePlayer.init();
	
	/* initialize command processors for LED related commands */
	setupLEDExecutors();	
}

Strobe* CommExecutorFacade::getStrobe()
{
	return &strobeChannel;
}

void CommExecutorFacade::initStrobeChannel()
{
	bool strobe_low_state_en = true;
	#ifdef FLASH_CHANNEL_LOW_STATE_IS_ENABLED
		strobe_low_state_en = true;
	#else
		strobe_low_state_en = false;
	#endif
	strobeChannel.initStrobe(FLASH_CHANNEL_DIRECTION_PORT,
							 FLASH_CHANNEL_PORT,
							 FLASH_CHANNEL_PIN_NO,
							 !strobe_low_state_en);
}

void CommExecutorFacade::setupLEDExecutors()
{	
	execLightSequence.setCommandCode(COMMAND_CODE_LIGHT_SEQUENCE);
	execLightSequence.setSequencePlayer(&ledLightsSequencePlayer);
	execChain.addExecutor(&execLightSequence);
	
	execChangeColor.setCommandCode(COMMAND_CODE_CHANGE_COLOR);
	execChain.addExecutor(&execChangeColor);
	
	strobeLightsExec.setCommandCode(COMMAND_STROBE_SEQUENCE);
	strobeLightsExec.setSequencePlayer(&strobePlayer);
	strobeLightsExec.setStrobe(&strobeChannel);
	execChain.addExecutor(&strobeLightsExec);
	
}
