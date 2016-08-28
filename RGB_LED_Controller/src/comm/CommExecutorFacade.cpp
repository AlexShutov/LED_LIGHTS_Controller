/*
* CommExecutorFacade.cpp
*
* Created: 6/18/2016 7:33:04 PM
* Author: Alex
*/


#include "CommExecutorFacade.h"
using namespace LedCommandExecutors;

// default constructor+
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
	initKeypad();
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
	/* and finally setup EE command executor */
	setupEECommandExectuor();
	// and, finally, setup executor, which can restore default
	// 'factory' :) light sequences in EEPROM. This call has to be
	// the last one, becayse EECommandExecutor initializes EEPlayer and
	// PresetsExecutor use EEPlayer for modifying data in EEPROM
	setupPresetExec();
}

void CommExecutorFacade::updateManually()
{
	strobePlayer.updateManually();
	ledLightsSequencePlayer.updateManually();
	// keypad use manual update with interrupts also
	keypad.updateManually();
}

EESupport::EEManager* CommExecutorFacade::getEEManager()
{
	return &eeManager;
}

EESupport::EEPlayer* CommExecutorFacade::getEEPlayer()
{
	return &eePlayer;
}

CommandExecutor* CommExecutorFacade::getExec()
{
	return &commandHistory;
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

void CommExecutorFacade::initKeypad()
{
	keypadCallback.setPlayer(getEEPlayer());
	keypad.setCallback(&keypadCallback);
	keypad.initialize();
}

void CommExecutorFacade::turnOn()
{
	// check if device has at least one saved sequence -
	// we need something to show after device intialization
	if (getEEPlayer()->getNumberOfCellsInUse() == 0){
		// there is no saved sequences, load
		// presets
		loadDefaultSequences();
	}
	// load data cell marked as current
	getEEPlayer()->reloadCurrentCell();
	keypad.setPlaybackState(true);
}

void CommExecutorFacade::turnOff()
{
	getEEPlayer()->stopBackgroundCommands();
	keypad.setPlaybackState(false);
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


void CommExecutorFacade::setupEECommandExectuor()
{
	// register EEPROM memory manager in 
	// EEPROM player.
	eePlayer.setEEManager(&eeManager);
	// register composite callback in EEPROM player
	// player will execute saved commands by it
	eePlayer.setCommandExec(&commandHistory);
	// set composite executor - player can stop all 
	// background commands if memory cell has no
	// saved background command. Player will ignore this
	// action if chain not set
	eePlayer.setExecChain(&execChain);
	// load data from EEPROM, called when all fields set
	eePlayer.init();
	// setup EECommandExecutor- it tells EEPlayer to
	// load command from cell or save command into it
	execEECommand.setCommandCode(COMMAND_EE);
	// register EEPROM player in EE command executor
	execEECommand.setEEPlayer(&eePlayer);
	// add ee command executor into execution chain
	// ee command is background command, so we don't
	// need to register it in command history manager
	execChain.addExecutor(&execEECommand);
}


void CommExecutorFacade::setupPresetExec()
{
	presetExecutor.setCommandCode(COMMAND_RESTORE_PRESETS);
	presetExecutor.setEEPlayer(&eePlayer);
	
	// set executors for testing
	presetExecutor.setExecChangeColor(&execChangeColor);
	presetExecutor.setColorSequenceExec(&execLightSequence);
	presetExecutor.setStrobeExec(&execStrobeLights);
	
}


void CommExecutorFacade::loadDefaultSequences()
{
	char buff[20];
	PresetDataStructure* pPresetsData = (PresetDataStructure*) buff;
	presetExecutor.setCommandCode(COMMAND_RESTORE_PRESETS);
	
	IncomingCommand command;
	command.setCommandCode(COMMAND_RESTORE_PRESETS);
	command.setBufferPtr(buff);
	command.setDataBlockSize(sizeof(PresetDataStructure));
	
	// tell executor to wipe out all data
	pPresetsData->wipeOutEEPROM = true;
	pPresetsData->restoreDefaults = true;
	// execute command
	presetExecutor.executeCommand(&command);
	
}