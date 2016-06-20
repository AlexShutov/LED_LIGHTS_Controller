/* 
* SequencePlayer.cpp
*
* Created: 6/19/2016 4:30:21 PM
* Author: Alex
*/


#include "SequencePlayer.h"
#include "../src/TimedPulse/TimeInterval.h"
#include "../src/TimedPulse/TimeInterval.h"

#include "../src/LED_RGB_Driver/RGB_Led.h"

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
SequencePlayer::SequencePlayer(uint8_t pulseGeneratorIndex)
{
	pulseIndex = pulseGeneratorIndex;
	init();
} //SequencePlayer

// default destructor
SequencePlayer::~SequencePlayer()
{
} //~SequencePlayer

void SequencePlayer::init(){
	currItemPosition = 0;
	totalItemCount = 0;
	isLooping = false;
	// clear this callback on startup, or it won't work otherwise
	pTerminationCallback = 0;
}



void SequencePlayer::useUniformItems(EventCallback* pItemCallback){
	isUniformItemsIsBeingUsed = true;
	// clear actions in buffer for multy-type sequence
	for (uint8_t i = 0; i < MAX_SEQUENCE_LENGTH; ++i){
		pDifferentItemsCallbacks[i].setDecoree(0);
		pDifferentItemsCallbacks[i].setActionAfter(0);
		pDifferentItemsCallbacks[i].setActionBefore(0);
	}
	// setup callback
	uniformCallback.setDecoree(pItemCallback);
	uniformCallback.setActionAfter(this);
	uniformCallback.setActionBefore(0);

}

void SequencePlayer::useItemsOfDifferentTypes(EventCallback* pItemsCallbackBuffer,
		uint8_t sequenceLength){
	isUniformItemsIsBeingUsed = false;
	uniformCallback.setDecoree(0);
	uniformCallback.setActionAfter(0);
	uniformCallback.setActionBefore(0);
	for (uint8_t i = 0; i < sequenceLength; i++){
		pDifferentItemsCallbacks[i].setDecoree(&pItemsCallbackBuffer[i]);
		pDifferentItemsCallbacks[i].setActionAfter(this);
		pDifferentItemsCallbacks[i].setActionBefore(0);
	}
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



void SequencePlayer::setupSequence(TimeInterval* pItemsDurations, 
			uint8_t sequenceLength,
			bool isLoopMode){
	pDurationOfItems = pItemsDurations;
	totalItemCount = sequenceLength;
	setLoopMode(isLoopMode);
	currItemPosition = 0;
	//pulseGenerator.addPulse(pItemsDurations, this, pulseIndex, true);
	
	EventCallback* pC = isUniformItemsIsBeingUsed ? &uniformCallback :
		pDifferentItemsCallbacks + 0;
	
		// set pulse number here (before callback is fired)
	pC->setPulseNo(0);
	pulseGenerator.addPulse(pDurationOfItems, pC, pulseIndex, true);
}

void SequencePlayer::onPulseStarted(){
}

/** process switching items here */
void SequencePlayer::onPulseEnded(){
	// sequence is ended
	/*
	if (currItemPosition == 0){
		Color c;
		c.red = 255;
		c.green = 255;
		c.blue = 255;
		RGB_Led::setColor(&c);
		return;
	}
	*/
	if (++currItemPosition >= totalItemCount){
		if (isLooping){
			currItemPosition = 0;
		} else {
			stopPlaying();
			// we're done, call terminate event here if there is any.
			if (pTerminationCallback){
				pTerminationCallback->onPulseStarted();
				pTerminationCallback->onPulseEnded();
			}
			return;
		}
	}
	
	TimeInterval* pNextItem = &pDurationOfItems[currItemPosition];
	// set pulse index in advance
	pulseGenerator.stopPulse(pulseIndex, false);
	EventCallback* pC = isUniformItemsIsBeingUsed ? &uniformCallback :
			&pDifferentItemsCallbacks[currItemPosition];
	pC->setPulseNo(currItemPosition);
	pulseGenerator.addPulse(pNextItem, pC, pulseIndex, true);
	
}

void SequencePlayer::setPulseNo(uint8_t pulseNo){}







