/*
 * TimedToggleSwitch.c
 *
 * Created: 25.12.2015 0:58:17
 *  Author: Alex
 */ 
#include "TimedPulse.h"
//#include "../src/LED_RGB_Driver/RGB_LED_Driver.h"

PulseTimings pulse_timings[MAX_PULSES_AT_A_TIME];
// prototype for an empty time interval;
TimeInterval empty_time_interval;
uint8_t uSecsCounter;

bool isZero(TimeInterval* pTimeInterval){
	return equals(pTimeInterval, &empty_time_interval);
}

void handleMillisecondTick(void);

ISR(TIMER0_COMPA_vect)
{
	uSecsCounter++;
	if (uSecsCounter == USEC_COUNT_INCS_PER_ONE_MILLISECOND - 1){
		uSecsCounter = 0;
		handleMillisecondTick();
	}
}



void setupTimedPulseTimer(){
	cli();
	
	// WGM01 enables clear on compare mode, cs01- pre-divider = 8 ( = 1MHz)
	
	TCCR0A = (0 << WGM00) | (1 << WGM01);
	TCCR0B =  (0 << CS00) | (1 << CS01) | (0 << CS02);
	TCNT0 = 0;

	// clear count register on compare (to 200)
	OCR0A = 200;
	TIMSK0 |= (1 << OCIE0A);
	
	sei();
	
}

void setupTimedPulse(void)
{	
	empty_time_interval.minutes = 0;
	empty_time_interval.seconds = 0;
	empty_time_interval.milliseconds = 0;
	// clear all pulse references;
	for (uint8_t i = 0; i < MAX_PULSES_AT_A_TIME; ++i){
		PulseTimings* pipt = &pulse_timings[i];
		pipt->isActive = false;
		pipt->duration = empty_time_interval;
		pipt->onPulseStartCallback = 0;
		pipt->onPulseEndCallback = 0;
	}
	setupTimedPulseTimer();
}

bool equals(TimeInterval* i1, TimeInterval* i2){
	return (i1->seconds == i2->seconds) &&
		   (i1->minutes == i2->minutes) &&
		   (i1->milliseconds == i2->milliseconds);
}

bool addPulse(TimeInterval* pDuration,
			  pCallback onPulseStartCallback,
			  pCallback onPulseEndCallback,
			  uint8_t pulse_index,
			  bool isActive)
{	
	PulseTimings* placeholder = &pulse_timings[pulse_index];
	// not active for now because we need to set callback first, interrupt
	// may occur
	placeholder->isActive = false;
	placeholder->duration = *pDuration;
	placeholder->currTime = empty_time_interval;
	// init callbacks
	placeholder->onPulseStartCallback = onPulseStartCallback;
	placeholder->onPulseEndCallback = onPulseEndCallback;
	// everyting is initialized, enable now if necessary
	if (isActive) {
		startPulse(pulse_index);
	}
}

void startPulse(uint8_t pulse_index) {
	PulseTimings* placeholder = &pulse_timings[pulse_index];
	placeholder->currTime = empty_time_interval;
	placeholder->isActive = true;
	placeholder->onPulseStartCallback(pulse_index);
}
void stopPulse(uint8_t pulse_index, bool fireEndCallback){
	PulseTimings* placeholder = &pulse_timings[pulse_index];
	placeholder->isActive = false;
	placeholder->currTime = empty_time_interval;
	if (fireEndCallback)
		placeholder->onPulseEndCallback(pulse_index);
}

void restartPulse(uint8_t pulse_index)
{
	stopPulse(pulse_index, true);
	startPulse(pulse_index);	
}

void handleMillisecondTick(void)
{
	
	for (uint8_t i = 0; i < MAX_PULSES_AT_A_TIME; ++i){
		PulseTimings* pt = &pulse_timings[i];
		if (!pt->isActive) continue;
		TimeInterval* pct = &pt->currTime;
				
		// increast current time value by 1 milisecond
		pct->milliseconds++;
		if (pct->milliseconds == 1000){
			pct->milliseconds = 0;
			pct->seconds++;
			if (pct->seconds == 60){
				pct->seconds = 0;
				pct->minutes++;
				// assume that light command doesn't support hours, in
				// that case just turn it on permanently
			}
		}
		bool isEqual = equals(&(pt->currTime), &pt->duration);
		if (isEqual){
			stopPulse(i, true);
		}
	}
}

void setPulseDuration(TimeInterval* pDuration, uint8_t pulse_index){
	PulseTimings* pPulse = &pulse_timings[pulse_index];
	if (pPulse->isActive){
		stopPulse(pulse_index, true);
	}
	pPulse->duration = *pDuration;
}

