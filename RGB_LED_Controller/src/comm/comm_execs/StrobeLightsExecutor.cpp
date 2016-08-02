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
/*		Inherited from  StrobeCallback                                  */
/************************************************************************/
StrobeCallback::StrobeCallback()
{
	pPulseDuratioins = 0;
	pPulsePauses = 0;
}

void StrobeCallback::setStrobe(Strobe* pStrobe)
{
	this->pStrobe = pStrobe;
}

void StrobeCallback::setStrobeData(TimeInterval* pDurations, 
								   TimeInterval* pPauses)
{
	pPulseDuratioins = pDurations;
	pPulsePauses = pPauses;
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
	CommandStrobesDataRecord* pCurrRec = 0;
	for (uint8_t i = 0; i < flashCnt; ++i){
		pCurrRec = pFirstRecord + i;
		// select pulse duration
		pT = &pCurrRec->flashDuration;
		pulseDurations[i] = *pT;
		// select pause
		pT = &pCurrRec->pauseDuration;
		pulsePauses[i] = *pT;
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
	pSequencePlayer->setLoopMode(dataHeader.repeat);
	
}

