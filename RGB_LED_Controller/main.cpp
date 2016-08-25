/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/io.h>

#include "compiler.h"
#include "hardware.h"
#include "../src/Utils/operators.h"
#include "../src/comm/CommExecutorFacade.h"

using TimeIntervalGeneration::TimeIntervalGenerator;

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
	// reload command saved as current in EEPROM
	facade.getEEPlayer()->reloadCurrentCell();
	while (1) 
    {		
		facade.pollForCommand();
		facade.updateManually();
    }
}

