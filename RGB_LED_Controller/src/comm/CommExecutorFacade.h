/* 
* CommExecutorFacade.h
*
* Created: 6/18/2016 7:33:04 PM
* Author: Alex
*/


#ifndef __COMMEXECUTORFACADE_H__
#define __COMMEXECUTORFACADE_H__

#include <avr/io.h>
#include "../src/comm/SourceOfData.h"
#include "../src/comm/comm_sources/UartSource.h"
#include "../src/comm/CommandReceiver.h"
#include "../src/comm/comm_execs/CompositeChainExecutor.h"
#include "../src/comm/comm_execs/ExecChangeColor.h"
#include "../src/comm/comm_execs/ColorSequenceExecutor.h"
#include "../src/TimedSequence/SequencePlayer.h"



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
	CommandReceiver commandReceiver;
	CompositeChainExecutor execChain;
	
	// responsible for rgb lights (there is stobes also)
	TimeIntervalGeneration::SequencePlayer ledLightsSequencePlayer;
	
	/** Executors responsible for LED commands */
	
	// COMMAND_CODE_CHANGE_COLOR
	LedCommandExecutors::ExecChangeColor execChangeColor;
	
	LedCommandExecutors::ExecChangeColor execChangeColor2;
	
	// COMMAND_CODE_LIGHT_SEQUENCE
	LedCommandExecutors::ColorSequenceExecutor execLightSequence;
	
	

//functions
public:
	CommExecutorFacade();
	~CommExecutorFacade();
	void pollForCommand();
	void initialize();
	
protected:
private:
	CommExecutorFacade( const CommExecutorFacade &c );
	CommExecutorFacade& operator=( const CommExecutorFacade &c );
	
	void setupLEDExecutors();

}; //CommExecutorFacade

#endif //__COMMEXECUTORFACADE_H__
