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
	// turn on device - this will load preset
	// sequences EEPROM is empty and select 
	// current sequence (or the first one if presets 
	// were loaded)
	facade.turnOn();
	while (1) {					
		facade.pollForCommand();
		facade.updateManually();
	}
}

