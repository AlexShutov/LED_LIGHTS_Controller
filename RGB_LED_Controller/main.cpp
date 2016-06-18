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
#include "../src/comm/UartSource.h"
#include "../src/comm/CommandReceiver.h"


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
	
	UartSource us;
	SourceOfData* ps  = &us;
	ps->initSource();
	CommandReceiver cr;
	cr.setSourceOfData(ps);
	
	while (1) 
    {		
		cr.receiveCommand();
    }
}

