/`	0	-+90??* 
* CommExecutorFacade.cpp
*
* Created: 6/18/2016 7:33:04 PM
* Author: Alex
*/


#include "CommExecutorFacade.h"


// default constructor+
*CommExecutorFacade::CommExecutorFacade()
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
	
	/* support for command history must be initialized before executors, because 
	   those use 'PausedCommandDecorator' as its end callback for restoring
	   paused command. This init method MUST be here, because commandReceiver hands
	   commands to commandHistory decorator
	*/
	initCommandHistorySupport();
	
	// bind command receiver to chain of commands
	commandReceiver.setCommandExecutor(&commandHistory);
	// connect command receiver to uart. if some command executor need to 
	// change data source, we can pass command receiver and uart data source to that
	// executor (for example, restoring and saving current command to eeprom)
	commandReceiver.setSourceOfData(&uart);
	
	// TimedPulse under index 0 generates timing pulses for RGB LED sequence 
	ledLightsSequencePlayer.setPulseGeneratorIndex(0);
	ledLightsSequencePlayer.setManualUpdate(true);
	ledLightsSequencePlayer.init();

	// TimedPulse with index 1 - handles strobe flashes
	strobePlayer.setPulseGeneratorIndex(1);
	strobePlayer.setManualUpdate(true);
	strobePlayer.init();
	
	
	
	/* initialize command processors for LED related commands */
	setupLEDExecutors();	
}

void CommExecutorFacade::updateManually()
{
	strobePlayer.updateManually();
	ledLightsSequencePlayer.updateManually();
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

void CommExecutorFacade::initCommandHistorySupport()
{
	// 'execChain' is a composite executor - it keep all real executors.
	commandHistory.setExecStorage(&execChain);
	commandHistory.setDecoree(&execChain);
	
}

void CommExecutorFacade::setupLEDExecutors()
{	
	execChangeColor.setCommandCode(COMMAND_CODE_CHANGE_COLOR);
	execChangeColor.setExternalEndCallback(&commandHistory);
	execChain.addExecutor(&execChangeColor);
	
	execLightSequence.setCommandCode(COMMAND_CODE_LIGHT_SEQUENCE);
	execLightSequence.setSequencePlayer(&ledLightsSequencePlayer);
	execLightSequence.setExternalEndCallback(&commandHistory);
	execChain.addExecutor(&execLightSequence);
	
	execStrobeLights.setCommandCode(COMMAND_STROBE_SEQUENCE);
	execStrobeLights.setSequencePlayer(&strobePlayer);
	execStrobeLights.setStrobe(&strobeChannel);
	//execStrobeLights.setExternalEndCallback(&commandHistory);
	execChain.addExecutor(&execStrobeLights);
	
}
