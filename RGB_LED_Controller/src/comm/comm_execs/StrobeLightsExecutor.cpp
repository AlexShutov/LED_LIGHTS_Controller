/* 
* FlashingLightSequence.cpp
*
* Created: 7/31/2016 11:32:56 PM
* Author: Alex
*/


#include "StrobeLightsExecutor.h"
#include "../src/hardware_drivers/RGB_Led.h"
using namespace StrobeRelated;
using namespace TimeIntervalGeneration;

void showOk()
{
	Color c;
	c.red = 0;
	c.green = 255;
	c.blue = 0;
	RGB_Led::setColor(&c);
}

void showError()
{
	Color c;
	c.red = 255;
	c.green = 0;
	c.blue = 0;
	RGB_Led::setColor(&c);
}

/************************************************************************/
/*		Inherited from  StrobeTerminateCallback                         */
/************************************************************************/

void StrobeTerminateCallback::onPulseStarted()
{
	showOk();
}

void StrobeTerminateCallback::onPulseEnded()
{
	Color c;
	c.red = 0;
	c.green = 0;
	c.blue = 0;
	RGB_Led::setColor(&c);
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

void StrobeCallback::onPulseEnded()
{
	uint8_t pulseIndex = getItemIndex();
	if (pulseIndex % 2 == 0){
		showError();
	}else {
		showOk();
	}
	
}

/************************************************************************/
/*		Inherited from  StrobeLightsExecutor                            */
/************************************************************************/

// default constructor
StrobeLightsExecutor::StrobeLightsExecutor()
{
} //FlashingLightSequence

// default destructor
StrobeLightsExecutor::~StrobeLightsExecutor()
{
} //~FlashingLightSequence

bool StrobeLightsExecutor::executeCommand(IncomingCommand* pCommand)
{
	pSequencePlayer->stopPlaying();
	loadDataFromCommand(pCommand);
	setupPlayerAndCallbacks();
}

bool StrobeLightsExecutor::revertCommand(IncomingCommand* pCommand)
{
	// stop playing strobe sequence
	pSequencePlayer->stopPlaying();
	// and turn off strobe channel
	pStrobe->turnOff();
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
	pSequencePlayer->setTerminationCallback(&terminateCallback);
	pSequencePlayer->setLoopMode(dataHeader.repeat);
	// we handle on and off alongside so *2
	uint8_t sequenceLen = 2 * dataHeader.numberOfFlashes;
	pSequencePlayer->setupSequence(strobeDurations, sequenceLen, 
			dataHeader.repeat);
	pStrobe->turnOn();
	showOk();
}

