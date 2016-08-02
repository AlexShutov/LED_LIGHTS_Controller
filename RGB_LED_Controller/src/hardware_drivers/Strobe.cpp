/* 
* Strobe.cpp
*
* Created: 7/31/2016 10:16:01 PM
* Author: Alex
*/


#include "Strobe.h"


// default constructor
Strobe::Strobe()
{
} //Strobe

// default destructor
Strobe::~Strobe()
{
} //~Strobe

void Strobe::initStrobe(volatile uint8_t* pDirPort, 
						volatile uint8_t* pPort, 
						uint8_t pinNo, 
						bool isHighStateOn)
{
	this->pDirPort = pDirPort;
	this->pPort = pPort;
	this->pinNo = pinNo;
	this->isHighStateOn = isHighStateOn;
	initDirPort();
	// turn off by default
	turnOff();
}

bool Strobe::isOn()
{
	return isNowOn;
}

void Strobe::turnOn()
{
	if (isHighStateOn){
		// set pin to high state
		(*pPort) |= _BV(pinNo);
	}else {
		// set pin to low state
		(*pPort) &= ~_BV(pinNo);
	}
	isNowOn = true;
}

void Strobe::turnOff()
{
	if (isHighStateOn){
		(*pPort) &= ~_BV(pinNo);
	} else {
		(*pPort) |= _BV(pinNo);
	}
	isNowOn = false;
}

void Strobe::initDirPort()
{
	(*pDirPort) |= _BV(pinNo);
}