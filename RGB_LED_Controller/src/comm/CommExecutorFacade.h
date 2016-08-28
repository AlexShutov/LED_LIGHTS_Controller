/* 
* CommExecutorFacade.h
*
* Created: 6/18/2016 7:33:04 PM
* Author: Alex
*/


#ifndef __COMMEXECUTORFACADE_H__
#define __COMMEXECUTORFACADE_H__

#include <avr/io.h>
#include "../src/hardware.h"
#include "../src/hardware_drivers/Strobe.h"
#include "../src/EEManager/EEManager.h"
#include "../src/comm/comm_execs/EE/EEPlayer.h"
#include "../src/comm/SourceOfData.h"
#include "../src/comm/comm_sources/UartSource.h"
#include "../src/comm/CommandReceiver.h"
#include "../src/comm/comm_execs/CompositeChainExecutor.h"
#include "../src/comm/comm_execs/ExecChangeColor.h"
#include "../src/comm/comm_execs/ColorSequenceExecutor.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"
#include "../src/comm/comm_execs/EECommandExecutor.h"
#include "../src/comm/comm_execs/PresetsCommandExecutor.h"
#include "../src/timed_sequence/SequencePlayer.h"
#include "../src/comm/PausedCommandDecorator.h"
#include "../src/hardware_drivers/keyboard/Keypad.h"
#include "../src/PlayerKeypadCallback.h"


/************************************************************************/
/* This is 'Facade' pattern- wraps setup and configuring of all 
/* command execution chain.
/************************************************************************/
class CommExecutorFacade 
{
//variables
public:
protected:
private:

	UartSource uart;
	Strobe strobeChannel;
	CommandReceiver commandReceiver;
	CompositeChainExecutor execChain;
	
	EESupport::EEManager eeManager;
	EESupport::EEPlayer  eePlayer;
	
	// responsible for rgb lights (there is strobes also)
	TimeIntervalGeneration::SequencePlayer ledLightsSequencePlayer;
	// plays strobe sequence
	TimeIntervalGeneration::SequencePlayer strobePlayer;
	
	/** Executors responsible for LED commands */
	
	// COMMAND_CODE_CHANGE_COLOR
	LedCommandExecutors::ExecChangeColor execChangeColor;
	
	// COMMAND_CODE_LIGHT_SEQUENCE
	LedCommandExecutors::ColorSequenceExecutor execLightSequence;
	
	// COMMAND_STROBE_SEQUENCE
	StrobeRelated::StrobeLightsExecutor execStrobeLights;
	
	// COMMAND_EE
	EESupport::EECommandExecutor execEECommand;
	
	// COMMAND_RESTORE_PRESETS
	LedCommandExecutors::PresetsCommandExecutor presetExecutor;
	
	// Support for the last paused command
	PausedCommandDecorator commandHistory;
	
	// KeypadSupport
	// keypad driver
	Keypad keypad;
	// callback, calling methods of EEPlayer whenever user
	// presses keypad button
	PlayerKeypadCallback keypadCallback;

	char buff[20];

//functions
public:
	CommExecutorFacade();
	~CommExecutorFacade();
	void pollForCommand();
	void initialize();
	
	void updateManually();
	
	void turnOn();
	void turnOff();
	
	void testPresetExec();
	
protected:
	CommandExecutor* getExec();
	Strobe* getStrobe();
	EESupport::EEManager* getEEManager();
	EESupport::EEPlayer* getEEPlayer();
	
private:
	CommExecutorFacade( const CommExecutorFacade &c );
	CommExecutorFacade& operator=( const CommExecutorFacade &c );
	/* Initialize strobe channel */
	void initStrobeChannel();
	void initCommandHistorySupport();
	void initKeypad();
	void setupLEDExecutors();
	// this is a very connected command, so its initialiation
	// is in separate method
	void setupPresetExec();
	void setupEECommandExectuor();
	

}; //CommExecutorFacade

#endif //__COMMEXECUTORFACADE_H__
