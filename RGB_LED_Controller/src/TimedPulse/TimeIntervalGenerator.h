/* 
* TimedPulseGenerator.h
*
* Created: 6/19/2016 12:39:13 PM
* Author: Alex
*/


#ifndef __TIMEDPULSEGENERATOR_H__
#define __TIMEDPULSEGENERATOR_H__
#include <avr/io.h>

extern "C" {
	#include "TimedPulse.h"
	#include "TimeInterval.h"
};

namespace TimeIntervalGeneration {
	

class EventCallback{
public:
	virtual void onPulseStarted() = 0;	
	virtual void onPulseEnded() = 0;
	// is very handy for handling sequence of pulses
	virtual void setPulseNo(uint8_t pulseNo) = 0;
};

class TimeIntervalGenerator
{
//variables
public:
protected:
private:
	
//functions
public:
	static void setupTimedPulse(void);
	
	bool addPulse(TimeInterval* pDuration, 
					EventCallback* pCallback, 
					uint8_t pulse_index,
					bool isActive);
	
	void startPulse(uint8_t pulse_index);
	void stopPulse(uint8_t pulse_index, bool fireEndCallback);
	void restartPulse(uint8_t pulse_index);
	void setPulseDuration(TimeInterval* pDuration, uint8_t pulse_index);
	
	TimeIntervalGenerator();
	~TimeIntervalGenerator();
protected:
private:
	TimeIntervalGenerator( const TimeIntervalGenerator &c );
	TimeIntervalGenerator& operator=( const TimeIntervalGenerator &c );

}; //TimedPulseGenerator

}
#endif //__TIMEDPULSEGENERATOR_H__
