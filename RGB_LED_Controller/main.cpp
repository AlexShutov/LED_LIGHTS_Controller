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

using TimeIntervalGeneration::TimeIntervalGenerator;

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
		c.green = 0;
		c.blue = 0;
		RGB_Led::setColor(&c);
	}
	
	virtual void onPulseEnded(){
		c.red = 0;
		c.green = 255;
		c.blue = 123;
		RGB_Led::setColor(&c);
	}
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
	TimeIntervalGenerator tg;
	
	TimeInterval ti;
	ti.milliseconds = 0;	
	ti.seconds = 0;
	ti.minutes = 1;
	Callback cb;
	
	tg.addPulse((TimeIntervalGeneration::TimeInterval*) &ti, &cb, 0, true);
	

	CommExecutorFacade facade;
	facade.initialize();
	
	while (1) 
    {		
		facade.pollForCommand();
		//commandReceiver.receiveCommand();
    }
}

