/* 
* SequencePlayer.h
*
* Created: 6/19/2016 4:30:21 PM
* Author: Alex
*/


#ifndef __SEQUENCEPLAYER_H__
#define __SEQUENCEPLAYER_H__

#include "../src/timed_pulse/TimeIntervalGenerator.h"
#include "../src/timed_pulse/EventCallbackDecorator.h"
#include "../src/comm/ManuallyUpdatable.h"

extern "C" {
	#include "../src/timed_pulse/TimeInterval.h"	
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
class SequencePlayer : protected EventCallback, public ManuallyUpdatable
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
	
	EventCallbackDecorator uniformCallback;
	// whether to play sequence in loop mode
	bool isLooping;
	// call this callback if we're not in loop mode and sequence ended
	EventCallback* pTerminationCallback;
	
	
//functions
public:
	SequencePlayer();
	~SequencePlayer();
	/* workflow is following - set pulseGeneratorIndex by 'setPulseGeneratorIndex'
	 and call 'init()' after it 
	 */
	void init();
	/* what TimedPulse should be used for this sequence */
	void setPulseGeneratorIndex(uint8_t pulseGeneratorIndex);
	// call during setup if you want all items to be of the same type
	void setIntervalEndCallback(EventCallback* pItemCallback);
	void setLoopMode(bool isLoopMode);
	void setTerminationCallback(EventCallback* pTerminationCallback);
	void stopPlaying();
	
	// switching between sequence items may take a while (longer than interval 
	// between another interrupts), so here is more coarse approach - use manual update 
	// from main loop
	
	virtual void updateManually();
	
	
	void setupSequence(TimeInterval* pItemsDurations, 
		uint8_t sequenceLength, 
		bool isLoopMode);
protected:

	virtual void onPulseStarted();
	virtual void onPulseEnded();
	virtual void setPulseNo(uint8_t pulseNo);
	
	
	
private:
	
	// set decorators for intercepting item callbacks
	
	SequencePlayer( const SequencePlayer &c );
	SequencePlayer& operator=( const SequencePlayer &c );
	
	void handlePulseEnd();

}; //SequencePlayer

}
#endif //__SEQUENCEPLAYER_H__
