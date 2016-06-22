/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/delay.h>
#include <avr/io.h>
#include "compiler.h"
#include "../src/Utils/operators.h"
#include "LED_RGB_Driver/RGB_Led.h"

#include "../src/comm/CommExecutorFacade.h"
#include "../src/TimedPulse/TimeIntervalGenerator.h"
#include "../src/TimedPulse/EventCallbackDecorator.h"
#include "../src/TimedSequence/SequencePlayer.h"
#include "../src/TimedPulse/EventCalbackCustomActions.h"

#include "../src/TimedSequence/Sequences/SequenceOfFlashes.h"

using TimeIntervalGeneration::TimeIntervalGenerator;
using TimeIntervalGeneration::EventCallbackDecorator;
using TimeIntervalGeneration::SequencePlayer;
using TimeIntervalGeneration::EventCalbackCustomActions;
using TimeIntervalGeneration::SequenceOfFlashes;

extern "C" {
	#include "../src/include/uart_stuff.h"
};



class ActionTurnOn : public TimeIntervalGeneration::EventCallback {
public:
	virtual void onPulseStarted(){
		PORTB |= _BV(5);
	}	
	virtual void onPulseEnded(){}
};

class ActionTurnOff : public TimeIntervalGeneration::EventCallback {
public:
	virtual void onPulseStarted(){
		PORTB &= ~_BV(5);
	}
	
	virtual void onPulseEnded(){}
};

#define outPin 3
void setupOutPin(){
	DDRB |= _BV(5);
}

int main(void)
{
	setupOutPin();
	// off
	//PORTB &= ~_BV(2);
	
	
	RGB_Led::init();
	uartInit();
	
	TimeIntervalGenerator::setupTimedPulse();
	
	ActionTurnOn actionOn;
	ActionTurnOff actionOff;
	
	actionOn.onPulseStarted();
	
	EventCalbackCustomActions customAction;
	customAction.setCustomAction(&actionOn, 0);
	customAction.setCustomAction(&actionOff, 1);
	
	
	TimeInterval durs[2];
	TimeInterval* pt = durs;
	pt->milliseconds = 0;
	pt->seconds = 0;
	pt->minutes = 5;
	pt++;
	
	pt->milliseconds = 0;
	pt->seconds = 0;
	pt->minutes = 10;
	
	SequencePlayer sp(0);
	sp.setIntervalEndCallback(&customAction);
	
	
	sp.setupSequence(durs, 2, true);
	
	
	CommExecutorFacade facade;
	facade.initialize();
	
	while (1) 
    {		
		//facade.pollForCommand();
		//commandReceiver.receiveCommand();
    }
}

