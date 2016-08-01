/* 
* A.cpp
*
* Created: 6/13/2016 9:51:55 PM
* Author: Alex
*/

extern "C" {
	#include "LED_RGB_Driver/RGB_LED_Driver.h"
	};

#include "RGB_Led.h"

// default constructor
RGB_Led::RGB_Led()
{
} //A

// default destructor
RGB_Led::~RGB_Led()
{
} //~A

void RGB_Led::init(void){
	initRgbLedDriver();
}

void RGB_Led::setColor(Color* pColor){
	/* assume that all fields is identical */
	::setColor((LEDColor*) pColor);
}

void RGB_Led::shiftToColor(Color* pColor){
	::setColor((LEDColor*) pColor);
}

void RGB_Led::nextRandomColor(void){
	::nextRandomColor();
}

Color* RGB_Led::getNextColor(){
	return (Color*) ::getNextColor();
}
