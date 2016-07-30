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
#include "../src/TimedPulse/EventCallbackCustomActions.h"

using TimeIntervalGeneration::TimeIntervalGenerator;
using TimeIntervalGeneration::EventCallbackDecorator;
using TimeIntervalGeneration::SequencePlayer;
using TimeIntervalGeneration::EventCallbackCustomActions;

extern "C" {
	#include "../src/include/uart_stuff.h"
};

Color c;
void f1(uint8_t pulseIndex){
	c.red = 255;
	c.green = 0;
	c.blue = 0;
	RGB_Led::setColor(&c);
}
void f2(uint8_t pulseIndex){
	c.red = 0;
	c.green = 255;
	c.blue = 0;
	RGB_Led::setColor(&c);
}

class Callback : public TimeIntervalGeneration::EventCallback {
public:
	virtual void onPulseStarted(){
		c.red = 0;
		c.green = 0;
		c.blue = 0;
		RGB_Led::setColor(&c);
	}
	
	virtual void onPulseEnded(){
		
	}
	virtual void setPulseNo(uint8_t pulseNo){}
	
};

class ColorCallback : public TimeIntervalGeneration::EventCallback {
public:
	virtual void onPulseStarted(){
		Color* pC = pColor + getItemIndex();
		RGB_Led::setColor(pC);
	}	
	
	virtual void onPulseEnded(){
		
	}
	
	void setColor(Color* pC, uint8_t size){
		pColor = pC;
		colorArSize = size;
	}
	
private:

	Color* pColor;
	uint8_t colorArSize;

};

Color cols[3];
ColorCallback colorCallback;
EventCallbackCustomActions customAction;
TimeInterval durs[3];
SequencePlayer sp;
Callback terminate;


void testSequence(){
	
	Color* pc = &cols[0];
	pc->red = 255;
	pc->green = 0;
	pc->blue = 0;
	pc++;
	
	pc->red = 0;
	pc->green = 255;
	pc->blue = 0;
	pc++;
	
	pc->red = 0;
	pc->green = 0;
	pc->blue = 255;
	
	colorCallback.setColor(cols, 3);
	
	//customAction.setCustomActions(colorCallback, 3);
	customAction.setCustomActionToMany(&colorCallback, MAX_CUSTOM_ACTIONS);
	//customAction.setCustomActions(&colorCallback, MAX_CUSTOM_ACTIONS);
	TimeInterval* pt = durs;
	pt->milliseconds = 100;
	pt->seconds = 0;
	pt->minutes = 0;
	pt++;
	
	pt->milliseconds = 100;
	pt->seconds = 0;
	pt->minutes = 0;
	pt++;
	
	pt->milliseconds = 100;
	pt->seconds = 0;
	pt->minutes = 0;
	
	sp.setPulseGeneratorIndex(0);
	sp.init();
	sp.setIntervalEndCallback(&customAction);
	sp.setTerminationCallback(&terminate);
	sp.setupSequence(durs, 3, true);
}

int main(void)
{
	RGB_Led::init();
	uartInit();
	
	TimeIntervalGenerator::setupTimedPulse();
	
	testSequence();
	
	
	CommExecutorFacade facade;
	facade.initialize();
	
	
	while (1) 
    {		
		facade.pollForCommand();
		//commandReceiver.receiveCommand();
    }
}

