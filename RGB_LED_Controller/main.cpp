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

using TimeIntervalGeneration::TimeIntervalGenerator;
using TimeIntervalGeneration::EventCallbackDecorator;
using TimeIntervalGeneration::SequencePlayer;

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
		c.red = 255;
		c.green = 255;
		c.blue = 255;
		RGB_Led::setColor(&c);
	}
	
	virtual void onPulseEnded(){
		
	}
	virtual void setPulseNo(uint8_t pulseNo){}
	
};

class ColorCallback : public TimeIntervalGeneration::EventCallback {
public:
	virtual void onPulseStarted(){
		Color* pC = pColor + getPulseNo();
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

int main(void)
{
	RGB_Led::init();
	uartInit();
	Color c;
	c.red = 0;
	c.green = 0;
	c.blue = 0;
    /* Replace with your application code */
	
	TimeIntervalGenerator::setupTimedPulse();
	
	TimeInterval ti;
	ti.milliseconds = 0;	
	ti.seconds = 0;
	ti.minutes = 1;
	
	Color cols[3];
	
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
	ColorCallback colorCallback[3];
	colorCallback[0].setColor(cols, 3);
	colorCallback[1].setColor(cols, 3);
	colorCallback[2].setColor(cols, 3);
	
	TimeInterval durs[3];
	TimeInterval* pt = durs;
	pt->milliseconds = 0;
	pt->seconds = 1;
	pt->minutes = 0;
	pt++;
	
	pt->milliseconds = 0;
	pt->seconds = 1;
	pt->minutes = 0;
	pt++;
	
	pt->milliseconds = 0;
	pt->seconds = 1;
	pt->minutes = 0;
	
	SequencePlayer sp(0);
	sp.setIntervalEndCallback(&colorCallback[1]);
	
	Callback terminate;
	//sp.setTerminationCallback(&terminate);
	sp.setupSequence(durs, 3, true);
	
	CommExecutorFacade facade;
	facade.initialize();
	
	while (1) 
    {		
		//facade.pollForCommand();
		//commandReceiver.receiveCommand();
    }
}

