/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/io.h>
#include "compiler.h"
#include "LED_RGB_Driver/RGB_Led.h"
#include <avr/delay.h>

extern "C" {
	#include "LED_RGB_Driver/RGB_LED_Driver.h"
};



int main(void)
{
	RGB_Led::init();
	LEDColor color;
	color.red = 50;
	color.green = 20;
	color.blue = 30;
	setColor(&color);
	
	Color c;
	c.red = 50;
	c.green = 0;
	c.blue = 0;
	RGB_Led::setColor(&c);
	
	
	
    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(5000);
		RGB_Led::nextRandomColor();
		
		
    }
}

