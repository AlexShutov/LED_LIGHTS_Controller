/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/delay.h>
#include <avr/io.h>

#include <avr/eeprom.h>

#include "compiler.h"
#include "hardware.h"
#include "../src/Utils/operators.h"
#include "hardware_drivers/RGB_Led.h"
#include "hardware_drivers/Strobe.h"
#include "../src/utils/Log.h"

#include "../src/comm/CommExecutorFacade.h"
#include "../src/timed_pulse/TimeIntervalGenerator.h"
#include "../src/timed_pulse/EventCallbackDecorator.h"
#include "../src/timed_sequence/SequencePlayer.h"
#include "../src/timed_pulse/EventCallbackCustomActions.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"
#include "../src/comm/PausedCommandDecorator.h"

#include "../src/EEManager/EEManager.h"
#include "../src/comm/comm_execs/EE/EEPlayer.h"

using TimeIntervalGeneration::TimeIntervalGenerator;
using TimeIntervalGeneration::EventCallbackDecorator;
using TimeIntervalGeneration::SequencePlayer;
using TimeIntervalGeneration::EventCallbackCustomActions;

using namespace LedCommandExecutors;
using namespace StrobeRelated;
using namespace EESupport;

extern "C" {
	#include "../src/include/uart_stuff.h"
};

void error(){
	Color c;
	Color::clear(&c);
	c.red = 255;
	RGB_Led::setColor(&c);
}
void ok(){
	Color c;
	Color::clear(&c);
	c.green = 255;
	RGB_Led::setColor(&c);
}


void whiteColor(){
	Color c;
	Color::clear(&c);
	c.red = 255;
	c.green = 255;
	c.blue = 255;
	RGB_Led::setColor(&c);
}



#define EEBUFFER_SIZE 200
#define EE_OFFSET 100



void testEEProm(EEManager* eeManager)
{
	Color c;
	Color::clear(&c);
	c.red = 245;
	c.green = 0;
	c.blue = 10;
	
	//eeManager->writeData(1, 10, &c, sizeof(Color));
	Color::clear(&c);
	
	eeManager->readData(1, 10, &c, sizeof(Color));
	RGB_Led::setColor(&c);
}


int main(void)
{
	RGB_Led::init();
	uartInit();
	TimeIntervalGenerator::setupTimedPulse();
	
	CommExecutorFacade facade;
	
	facade.initialize();
	
	testEEProm(facade.getEEManager());
	
	EEPlayer* pPlayer = facade.getEEPlayer();
	
	
	PausedCommandDecorator pcd;
	while (1) 
    {		
		facade.pollForCommand();
		facade.updateManually();
		//commandReceiver.receiveCommand();
    }
}

