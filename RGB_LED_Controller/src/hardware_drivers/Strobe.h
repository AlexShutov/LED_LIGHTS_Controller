/* 
* Strobe.h
*
* Created: 7/31/2016 10:16:01 PM
* Author: Alex
*/


#ifndef __STROBE_H__
#define __STROBE_H__

#include "compiler.h"

class Strobe
{
//variables
public:
protected:
private:
	volatile uint8_t* pDirPort;
	volatile uint8_t* pPort;
	uint8_t pinNo;
	bool isHighStateOn;
	bool isNowOn;
	
//functions
public:
	void initStrobe(volatile uint8_t* pDirPort,
					volatile uint8_t* pPort,
					uint8_t pinNo, 
					bool isHighStateOn);
	Strobe();
	~Strobe();
	
	bool isOn();
	void turnOn();
	void turnOff();
	
protected:
private:
	Strobe( const Strobe &c );
	Strobe& operator=( const Strobe &c );
	
	void initDirPort();

}; //Strobe

#endif //__STROBE_H__
