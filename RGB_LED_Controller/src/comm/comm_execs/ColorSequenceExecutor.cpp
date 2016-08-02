/* 
* LightSequenceExecutor.cpp
*
* Created: 7/29/2016 3:25:34 AM
* Author: Alex
*/


#include "ColorSequenceExecutor.h"

using namespace LedCommandExecutors;

/************************************************************************/
/*		 Methods of SequenceTerminateCallback                           */
/************************************************************************/

void SequenceTerminateCallback::onPulseStarted(){
	Color::clear(&c);
	RGB_Led::setColor(&c);
}

void SequenceTerminateCallback::onPulseEnded()
{
}

void SequenceTerminateCallback::setPulseNo(uint8_t pulseNo)
{
}

/************************************************************************/
/*		  Methods of ColorCallback                                      */
/************************************************************************/

void ColorCallback::onPulseStarted(){
	// get pointer to current color
	Color* pC = pColor + getItemIndex();
	if (!isSmoothSwitching){
		RGB_Led::setColor(pC);
	} else {
		RGB_Led::shiftToColor(pC);
	}
}

void ColorCallback::onPulseEnded()
{
}

void ColorCallback::setIsSmoothSwitching(bool isSmooth){
	isSmoothSwitching = isSmooth;
}

void ColorCallback::setColor(Color* pC, uint8_t size){
	pColor = pC;
	colorArSize = size;
}



/************************************************************************/
/*       Methods of ColorSequenceExecutor
/************************************************************************/

// default constructor
ColorSequenceExecutor::ColorSequenceExecutor()
{
	Color::clear(&colorOff);
	for (uint8_t i = 0; i < MAX_SEQUENCE_LENGTH; ++i){
		TimeInterval* pT = intervalDurations + i;
		pT->milliseconds = 0;
		pT->seconds = 0;
		pT->minutes = 0;
		Color::clear(intervalColors + i);
	}
	// bind ColorCallback to color data once
	colorCallback.setColor(intervalColors, MAX_SEQUENCE_LENGTH);
	colorCallback.setIsSmoothSwitching(false);
	compositeCallback.setCustomActionToMany(&colorCallback, MAX_SEQUENCE_LENGTH);
} //LightSequenceExecutor

// default destructor
ColorSequenceExecutor::~ColorSequenceExecutor()
{
} //~LightSequenceExecutor

void ColorSequenceExecutor::setSequencePlayer(TimeIntervalGeneration::SequencePlayer* pPlayer){
	pSequencPlayer = pPlayer;
}

TimeInterval* ColorSequenceExecutor::getTimeInterval(uint8_t index){
	CommColorSequenceRecord* pRec = pDataRecords + index;
	TimeInterval* pDuration = &(pRec->pulseDuration);
	return pDuration;
}

/*
	Here is a tricky part: There is several RGB LED commands, each having its
	own structure of data record, but only one SequencePlayer. That mean, if we want to
	switch command executor, we should disable current executor first, and then ask another
	to execute command - it will set itself as mapper in SequencePlayer. Imaging, if we
	start executing another command while the old one is still running- SequencePlayer
	would have old data, but new data structure. 
*/
bool ColorSequenceExecutor::executeCommand(IncomingCommand* pCommand){
	// this is mandatory - see comment above
	pSequencPlayer->stopPlaying();
	// Use this executor as TimeInterval mapper
	loadData(pCommand);
	setupPlayerAndCallbacks();
}
/* Stop playing current sequence and turn off RGB LED */
bool ColorSequenceExecutor::revertCommand(IncomingCommand* pCommand){
	pSequencPlayer->stopPlaying();
	RGB_Led::setColor(&colorOff);
}

void ColorSequenceExecutor::loadData(IncomingCommand* pCommand){
	char* pBuffer = pCommand->getBufferPtr();
	CommColorHeader* pDataHeader = (CommColorHeader*) pBuffer;
	dataHeader = * pDataHeader;
	bool isLooping = dataHeader.repeat;
	// section with color data comes right after command data header
	pDataRecords = (CommColorSequenceRecord*) ( pDataHeader + 1);
	for (uint8_t i = 0; i < pDataHeader->numberOfLights; ++i){
		CommColorSequenceRecord* pCurrRec = pDataRecords + i;
		// point to current time interval
		TimeInterval* pT = intervalDurations + i;
		// copy it
		*pT = pCurrRec->pulseDuration;
		// copy color data
		Color* pC = intervalColors + i;
		*pC = pCurrRec->pulseColor;
	}
}

void ColorSequenceExecutor::setupPlayerAndCallbacks()
{
	colorCallback.setIsSmoothSwitching(dataHeader.isSmoothSwitch);
	pSequencPlayer->init();
	pSequencPlayer->setLoopMode(dataHeader.repeat);
	// set interval and end callbacks, specific to this command type
	pSequencPlayer->setIntervalEndCallback(&compositeCallback);
	pSequencPlayer->setTerminationCallback(&terminateCallback);
	pSequencPlayer->setupSequence(intervalDurations, dataHeader.numberOfLights,
		dataHeader.repeat);
}