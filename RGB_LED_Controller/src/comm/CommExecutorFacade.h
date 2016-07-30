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
#include "../src/comm/comm_execs/LightSequenceExecutor.h"
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
	
	
	
	/** Executors responsible for LED commands */
	LedCommandExecutors::ExecChangeColor commChangeColor;
	LedCommandExecutors::LightSequenceExecutor commLightSequence;

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
