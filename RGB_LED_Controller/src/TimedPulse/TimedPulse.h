/*
 * TimedToggleSwitch.h
 *
 * Created: 25.12.2015 0:58:04
 *  Author: Alex
 */ 


#ifndef TIMEDTOGGLESWITCH_H_
#define TIMEDTOGGLESWITCH_H_
#include <avr/io.h>
#include "compiler.h"

typedef struct {
	uint16_t milliseconds;
	uint8_t seconds;
	uint8_t minutes;
}TimeInterval;
bool equals(TimeInterval* i1, TimeInterval* i2);
bool isZero(TimeInterval* pTimeInterval);

void setupTimedPulseTimer();

// callback function for time interval expiration event
typedef void (*pCallback)(uint8_t pulseIndex);

typedef struct {
	TimeInterval duration;
	TimeInterval  currTime;
	bool isActive;
	pCallback onPulseStartCallback;
	pCallback onPulseEndCallback;
	// whether now is high state of pulse or low
} PulseTimings;

// one pulse controls RGB- channel and the two others - white LEDs
#define MAX_PULSES_AT_A_TIME 2
/* Atmega128A have had frequency 8MHz, Now I user Atmega328P (Chinese Arduino pro mini- it's smaller)
which has frequency 16MHz
#define USEC_COUNT_INCS_PER_ONE_MILLISECOND 5
*/
#define USEC_COUNT_INCS_PER_ONE_MILLISECOND 10 // 1/8 = 1MHZ (200uSec per counter tick)
void setupTimedPulse(void);

/** add a new pulse */
bool addPulse(TimeInterval* pDuration,
	pCallback onPulseStartCallback,
	pCallback onPulseEndCallback,
	uint8_t pulse_index,
	bool isActive);

void startPulse(uint8_t pulse_index);
void stopPulse(uint8_t pulse_index, bool fireEndCallback);
void restartPulse(uint8_t pulse_index);
// we don't restart pulse here, because this func is meant to be used during
// switch of lights and we also need change the LED color.
void setPulseDuration(TimeInterval* pDuration, uint8_t pulse_index);


#endif /* TIMEDTOGGLESWITCH_H_ */