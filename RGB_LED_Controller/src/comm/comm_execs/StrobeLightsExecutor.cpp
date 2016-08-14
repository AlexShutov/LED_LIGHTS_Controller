/* 
* FlashingLightSequence.cpp
*
* Created: 7/31/2016 11:32:56 PM
* Author: Alex
*/


#include "StrobeLightsExecutor.h"
using namespace StrobeRelated;
using namespace TimeIntervalGeneration;


/************************************************************************/
/*		Inherited from  StrobeTerminateCallback                         */
/************************************************************************/

/* 
 This is a case when sequence is repeatable and it has just reached the end
 and must be started again - turn on strobe channel
 */
void StrobeTerminateCallback::onPulseStarted()
{
	pStrobe->turnOn();
}

/* 
	Strobe sequence ended and is not in 'loop' mode -
	turn off strobe channel
*/
void StrobeTerminateCallback::onPulseEnded()
{
	pStrobe->turnOff();
}

void StrobeTerminateCallback::setStrobe(Strobe* pStrobe)
{
	this->pStrobe = pStrobe;
}

/************************************************************************/
/*		Inherited from  StrobeCallback                                  */
/************************************************************************/
StrobeCallback::StrobeCallback()
{
	pData = 0;
}

void StrobeCallback::setStrobe(Strobe* pStrobe)
{
	this->pStrobe = pStrobe;
}

void StrobeCallback::setStrobeData(TimeInterval* pData)
{
	this->pData = pData;
}

void StrobeCallback::onPulseStarted()
{
}

/*
	This callback handles end of a pulse. Id we processing
	end of even pulse - strobe ended, turn channel off, and
	otherwise - turn on for odd pulse index
*/
void StrobeCallback::onPulseEnded()
{
	uint8_t pulseIndex = getItemIndex();
	if (pulseIndex % 2 == 0){
		pStrobe->turnOff();
	}else {
		pStrobe->turnOn();
	}
	
}

/************************************************************************/
/*		Inherited from  StrobeLightsExecutor                            */
/************************************************************************/

// default constructor
StrobeLightsExecutor::StrobeLightsExecutor()
{
	terminateDispatcher.setExec(this);
	setEndCallbackDispatcher(&terminateDispatcher);
	terminateDispatcher.setEndAction(&terminateCallback);
	
} //FlashingLightSequence

// default destructor
StrobeLightsExecutor::~StrobeLightsExecutor()
{
} //~FlashingLightSequence

bool StrobeLightsExecutor::executeCommand(IncomingCommand* pCommand)
{
	// no call to 'resumeCommand' because player has to be stopped before
	// re-scheduling playing and loading command data and if we call  
	//'resumeCommand' it will be done twice
	pSequencePlayer->stopPlaying();
	loadDataFromCommand(pCommand);
	setupPlayerAndCallbacks();
	return true;
}

bool StrobeLightsExecutor::isRGBCommand()
{
	return false;
}

bool StrobeLightsExecutor::isCommandResumable()
{
	return false;
}

bool StrobeLightsExecutor::stopCommand(uint8_t commandCode)
{
	if (commandCode != getCommandCode()){
		return false;
	}
	// stop playing strobe sequence
	pSequencePlayer->stopPlaying();
	// and turn off strobe channel
	pStrobe->turnOff();
	return true;
}

bool StrobeLightsExecutor::resumeCommand(uint8_t commandCode)
{
	if (commandCode != getCommandCode()){
		return false;
	}
	pSequencePlayer->stopPlaying();
	setupPlayerAndCallbacks();
	return true;
}

void StrobeLightsExecutor::setStrobe(Strobe* pStrobe){
	this->pStrobe = pStrobe;
	callback.setStrobe(pStrobe);
	terminateCallback.setStrobe(pStrobe);
}

void StrobeLightsExecutor::setSequencePlayer(SequencePlayer* pPlayer)
{
	this->pSequencePlayer = pPlayer;
}

void StrobeLightsExecutor::loadDataFromCommand(IncomingCommand* pCommand)
{
	CommandStrobesDataHeader* pHeader = (CommandStrobesDataHeader*) pCommand->getBufferPtr();
	// copy command header
	dataHeader = *pHeader;
	// don't copy any data if we just need to turn on/off strobe channel
	if (pHeader->isItPermanent){
		return;
	}	
	CommandStrobesDataRecord* pFirstRecord = (CommandStrobesDataRecord*)(pHeader + 1);
	// number of pulses
	uint8_t flashCnt = pHeader->numberOfFlashes;
	// copy durations of pulses and pauses to corresponding arrays
	TimeInterval* pT = 0;
	TimeInterval* pDest = strobeDurations;
	CommandStrobesDataRecord* pCurrRec = 0;
	for (uint8_t i = 0; i < flashCnt; ++i){
		// point to current data record
		pCurrRec = pFirstRecord + i;
		// point to flash duration
		pT = &pCurrRec->flashDuration;
		// copy it
		*pDest = *pT;
		// move forward
		pDest++;
		// copy pause interval
		pT = &pCurrRec->pauseDuration;
		*pDest = *pT;
		pDest++;
	}
}

void StrobeLightsExecutor::setupPlayerAndCallbacks()
{
	if (dataHeader.isItPermanent){
		// turn on/off channel and finish
		if (dataHeader.isON){
			pStrobe->turnOn();
		} else {
			pStrobe->turnOff();
		}
		return;
	}
	pSequencePlayer->init();
	// update data within callback
	callback.setStrobeData(strobeDurations);
	// here we have to know pulse number only, so no need
	// in composite callback 'CallbackCustomActions'
	pSequencePlayer->setIntervalEndCallback(&callback);
	pSequencePlayer->setTerminationCallback(&terminateDispatcher);
	pSequencePlayer->setLoopMode(dataHeader.repeat);
	// we handle on and off alongside so *2
	uint8_t sequenceLen = 2 * dataHeader.numberOfFlashes;
	pSequencePlayer->setupSequence(strobeDurations, sequenceLen, 
			dataHeader.repeat);
	pStrobe->turnOn();
}

