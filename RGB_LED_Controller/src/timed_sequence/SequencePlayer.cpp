/* 
* SequencePlayer.cpp
*
* Created: 6/19/2016 4:30:21 PM
* Author: Alex
*/


#include "SequencePlayer.h"
#include "../src/timed_pulse/TimeInterval.h"
#include "../src/timed_pulse/TimeInterval.h"

#include "../src/hardware_drivers/RGB_Led.h"

using namespace TimeIntervalGeneration;


/************************************************************************/
/* SequenceItem's methods
/************************************************************************/
void SequenceItem::setDuration(TimeInterval* pTimeInterval){
	duration = *pTimeInterval;
	
}

void SequenceItem::setItemAction(EventCallback* pAction){
	itemAction.setDecoree(pAction);
}

void SequenceItem::setActionAfter(EventCallback* pAfterAction){
	itemAction.setActionAfter(pAfterAction);
}

EventCallback* SequenceItem::getAction(){
	
}

/************************************************************************/
/*  SequencePlayer's methods
/************************************************************************/

// default constructor
SequencePlayer::SequencePlayer()
:ManuallyUpdatable()
{
}

// default destructor
SequencePlayer::~SequencePlayer()
{
} //~SequencePlayer

void SequencePlayer::setPulseGeneratorIndex(uint8_t pulseGeneratorIndex){
	pulseIndex = pulseGeneratorIndex;
}
void SequencePlayer::init(){
	currItemPosition = 0;
	totalItemCount = 0;
	isLooping = false;
	// clear this callback on startup, or it won't work otherwise
	pTerminationCallback = 0;
}

void SequencePlayer::setIntervalEndCallback(EventCallback* pItemCallback){
	// clear actions in buffer for multi-type sequence
	// setup callback
	uniformCallback.setDecoree(pItemCallback);
	/* SequencePlayer extend EventCallback by itself for handling transitions between
		sequence items */
	uniformCallback.setActionAfter(this);
	/* Player only handles transition to next item, here is no need in 'actionAfter' */
	uniformCallback.setActionBefore(0);
}

void SequencePlayer::setLoopMode(bool isLoopMode){
	isLooping = isLoopMode;
}

void SequencePlayer::setTerminationCallback(EventCallback* pTerminationCallback){
	this->pTerminationCallback = pTerminationCallback;
}

void SequencePlayer::stopPlaying(){
	// fire end callback (to stop current item)
	pulseGenerator.stopPulse(pulseIndex, false);
	// reset current position
	currItemPosition = 0;
}


void SequencePlayer::updateManually()
{
	if (isManualUpdate() && isUpdateNeeded()){
		handlePulseEnd();
		markAsUpdated();
	}
}




void SequencePlayer::setupSequence(TimeInterval* pItemsDurations, 
			uint8_t sequenceLength,
			bool isLoopMode){
	pDurationOfItems = pItemsDurations;
	totalItemCount = sequenceLength;
	setLoopMode(isLoopMode);
	currItemPosition = 0;
	//pulseGenerator.addPulse(pItemsDurations, this, pulseIndex, true);
	
	
	// set pulse number here (before callback is fired)
	uniformCallback.setItemIndex(0);
	pulseGenerator.addPulse(pDurationOfItems, &uniformCallback, pulseIndex, true);
}

void SequencePlayer::onPulseStarted(){
}

/** process switching items here */
void SequencePlayer::onPulseEnded(){
	if (!isManualUpdate()){
		handlePulseEnd();
	} else {
		markAsNotUpdated();
	}
}

void SequencePlayer::handlePulseEnd()
{
	// sequence is ended
	if (++currItemPosition >= totalItemCount){
		if (isLooping){
			currItemPosition = 0;
			} else {
			//stopPlaying();
			// we're done, call terminate event here if there is any.
			if (pTerminationCallback){
				//pTerminationCallback->onPulseStarted();
				pTerminationCallback->onPulseEnded();
			}
			return;
		}
	}
	TimeInterval* pNextItem = 0;
	pNextItem = &pDurationOfItems[currItemPosition];
	// set pulse index in advance
	//pulseGenerator.stopPulse(pulseIndex, false);
	
	uniformCallback.setItemIndex(currItemPosition);
	pulseGenerator.addPulse(pNextItem, &uniformCallback, pulseIndex, true);
	
}

void SequencePlayer::setPulseNo(uint8_t pulseNo){}







