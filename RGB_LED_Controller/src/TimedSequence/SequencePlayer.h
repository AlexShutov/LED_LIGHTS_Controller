/* 
* SequencePlayer.h
*
* Created: 6/19/2016 4:30:21 PM
* Author: Alex
*/


#ifndef __SEQUENCEPLAYER_H__
#define __SEQUENCEPLAYER_H__

#include "../src/TimedPulse/TimeIntervalGenerator.h"
#include "../src/TimedPulse/EventCallbackDecorator.h"

extern "C" {
	#include "../src/TimedPulse/TimeInterval.h"	
};


namespace TimeIntervalGeneration {

// size of callback buffer
#define MAX_SEQUENCE_LENGTH 20
	

/* Sequence may contain intervals with different actions 
 (not only color change). Action is defined by callback, wrapped in
 EvenCallbackDecorator. Decoration is used for checking if sequence has 
 next elements and if id doesn't - whether to replay entire sequence
*/
class SequenceItem {
private:
	TimeInterval duration;
	EventCallbackDecorator itemAction;
public:
	void setDuration(TimeInterval* pDuration);
	TimeInterval* getDuration();
	
	void setItemAction(EventCallback* pAction);
	void setActionAfter(EventCallback* pAfterAction);
	EventCallback* getAction();

};

/************************************************************************/
/* This class extend abstract class EventCallback for moving 
/* between items of sequence
/************************************************************************/
class SequencePlayer : protected EventCallback
{
	
//variables
public:
protected:
private:
	TimeIntervalGenerator pulseGenerator;
	// index of pulse assigned to this player (used by TimeIntervalGenerator) 
	uint8_t pulseIndex;
	// position of currently playing item in sequence
	uint8_t currItemPosition;
	// how many items sequence has
	uint8_t totalItemCount;
	
	// we need to store sequence's data somewhere. We can do it right here, but 
	// in that case we'll need to copy entire sequence before playing. We'll use
	// double buffering instead.
	TimeInterval* pDurationOfItems;
	
	// we may want to use uniform items (only color change, for example)
	// in that case we won't need to store array of callbacks, only one callback
	// is needed
	bool isUniformItemsIsBeingUsed;
	EventCallbackDecorator uniformCallback;
	// But if you want to use different items as sequence elements, provide
	// callback buffer, which size is the same, as totalItemCount to
	// useItemsOfDifferentTypes() method
	EventCallbackDecorator pDifferentItemsCallbacks[MAX_SEQUENCE_LENGTH];
	// whether to play sequence in loop mode
	bool isLooping;
	// call this callback if we're not in loop mode and sequence ended
	EventCallback* pTerminationCallback;
	
//functions
public:
	// pulse index should not be changed
	SequencePlayer(uint8_t pulseGeneratorIndex);
	~SequencePlayer();
	
	// call during setup if you want all items to be of the same type
	void useUniformItems(EventCallback* pItemCallback);
	void useItemsOfDifferentTypes(EventCallback* pItemsCallbackBuffer, uint8_t sequenceLength);
	void setLoopMode(bool isLoopMode);
	void setTerminationCallback(EventCallback* pTerminationCallback);
	void stopPlaying();

	void setupSequence(TimeInterval* pItemsDurations, 
		uint8_t sequenceLength, 
		bool isLoopMode);
protected:

	virtual void onPulseStarted();
	virtual void onPulseEnded();
	virtual void setPulseNo(uint8_t pulseNo);
	
private:
	void init();
	// set decorators for intercepting item callbacks
	
	SequencePlayer( const SequencePlayer &c );
	SequencePlayer& operator=( const SequencePlayer &c );

}; //SequencePlayer

}
#endif //__SEQUENCEPLAYER_H__
