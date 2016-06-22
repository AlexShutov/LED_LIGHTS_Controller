/* 
* SequenceOfFlashes.h
*
* Created: 6/21/2016 2:37:29 AM
* Author: Alex
*/


#ifndef __SEQUENCEOFFLASHES_H__
#define __SEQUENCEOFFLASHES_H__

#include <avr/io.h>
#include "../src/TimedSequence/SequencePlayer.h"
#include "../src/TimedPulse/EventCalbackCustomActions.h"

namespace TimeIntervalGeneration {


class SequenceOfFlashes : protected SequencePlayer
{
//variables
public:
protected:

	EventCalbackCustomActions storedPulses;
private:
	uint8_t totalPulses;
	TimeInterval pulseDurations[MAX_CUSTOM_ACTIONS / 2];
	TimeInterval pauseDurations[MAX_CUSTOM_ACTIONS / 2];
	
	
//functions
public:
	SequenceOfFlashes(uint8_t pulseIndex);
	~SequenceOfFlashes();
	
	
	
protected:
private:
	SequenceOfFlashes( const SequenceOfFlashes &c );
	SequenceOfFlashes& operator=( const SequenceOfFlashes &c );

}; //SequenceOfFlashes
}

#endif //__SEQUENCEOFFLASHES_H__
