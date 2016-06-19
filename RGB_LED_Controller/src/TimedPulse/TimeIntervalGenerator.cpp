/* 
* TimedPulseGenerator.cpp
*
* Created: 6/19/2016 12:39:13 PM
* Author: Alex
*/


#include "TimeIntervalGenerator.h"
extern "C" {
	#include "../src/TimedPulse/TimedPulse.h"
};


using TimeIntervalGeneration::TimeIntervalGenerator;
using TimeIntervalGeneration::EventCallback;

EventCallback* callbacksForPulses[MAX_PULSES_AT_A_TIME];

void pulseStartCallbackWrapper(uint8_t pulseIndex){
	EventCallback* pCallback = callbacksForPulses[pulseIndex];
	pCallback->onPulseStarted();
}

void pulseEndCallbackWrapper(uint8_t pulseIndex){
	EventCallback* pCallback = callbacksForPulses[pulseIndex];
	pCallback->onPulseEnded();
}

void TimeIntervalGenerator::setupTimedPulse(){
	for (uint8_t i = 0; i < MAX_PULSES_AT_A_TIME; ++i){
		callbacksForPulses[i] = 0;
	}
	::setupTimedPulse();	
}


// default constructor
TimeIntervalGenerator::TimeIntervalGenerator()
{
} //TimedPulseGenerator

// default destructor
TimeIntervalGenerator::~TimeIntervalGenerator()
{
} //~TimedPulseGenerator

bool TimeIntervalGenerator::addPulse(TimeIntervalGeneration::TimeInterval* pDuration, 
			EventCallback* pCallback,
			uint8_t pulse_index, 
			bool isActive)
{
	callbacksForPulses[pulse_index] = pCallback;
	::addPulse((::TimeInterval*) pDuration, pulseStartCallbackWrapper,
				pulseEndCallbackWrapper, pulse_index, isActive);	
}

void TimeIntervalGenerator::startPulse(uint8_t pulse_index){
	::startPulse(pulse_index);
}

void TimeIntervalGenerator::stopPulse(uint8_t pulse_index, bool fireEndCallback){
	::stopPulse(pulse_index, fireEndCallback);
}
void TimeIntervalGenerator::restartPulse(uint8_t pulse_index){
	::restartPulse(pulse_index);
}

void TimeIntervalGenerator::setPulseDuration(TimeInterval* pDuration, 
									uint8_t pulse_index){
		::setPulseDuration((::TimeInterval*) pDuration, pulse_index);
}
