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
#include "../src/comm/SourceOfData.h"
#include "../src/comm/comm_sources/UartSource.h"
#include "../src/comm/CommandReceiver.h"

#include "../src/comm/comm_execs/CompositeChainExecutor.h"
#include "../src/comm/comm_execs/ExecChangeColor.h"


extern "C" {
	#include "../src/include/uart_stuff.h"
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
	
	UartSource uartSource;
	SourceOfData* dataSource  = &uartSource;
	dataSource->initSource();
	
	// create command receiver and executor chain
	CommandReceiver commandReceiver;
	CompositeChainExecutor execChain;
	ExecChangeColor commChangeColor;
	
	execChain.addExecutor(&commChangeColor);
	
	// register data source and exec chain in command receiver
	commandReceiver.setSourceOfData(dataSource);
	commandReceiver.setCommandExecutor(&execChain);
	
	while (1) 
    {		
		commandReceiver.receiveCommand();
    }
}

