/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include "compiler.h"
#include "hardware.h"
#include "../src/Utils/operators.h"
#include "../src/comm/CommExecutorFacade.h"

#include "../src/hardware_drivers/RGB_Led.h"


using TimeIntervalGeneration::TimeIntervalGenerator;
using namespace TimeIntervalGeneration;
using namespace LedCommandExecutors;

extern "C" {
	#include "../src/include/uart_stuff.h"
};

Color c;
void ok(){
	Color::clear(&c);
	c.green = 255;
	RGB_Led::setColor(&c);
}

void error(){
	Color::clear(&c);
	c.red = 255;
	RGB_Led::setColor(&c);
}

char buff[20];
int main(void)
{
	// initialize hardware drivers
	RGB_Led::init();
	uartInit();
	TimeIntervalGenerator::setupTimedPulse();
	// create and initialize device facade (see
	// pattern description in GOF)
	CommExecutorFacade facade;
	facade.initialize();
	// reload command saved as current in EEPROM
	//facade.getEEPlayer()->reloadCurrentCell();
	// turn it off for now - until 'Presets' executor not ready
	facade.turnOn();
	
	
	facade.testPresetExec();
	
	while (1) 
    {		
			
		facade.pollForCommand();
		facade.updateManually();
		
	}
}

