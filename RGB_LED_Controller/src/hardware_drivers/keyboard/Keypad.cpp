/* 
* Keypad.cpp
*
* Created: 8/27/2016 5:34:24 PM
* Author: Alex
*/


#include <interrupt.h>
#include <avr/io.h>
#include "compiler.h"
#include "../src/hardware.h"
#include "Keypad.h"


// default constructor
Keypad::Keypad()
{
} //Keypad

// default destructor
Keypad::~Keypad()
{
} //~Keypad

void Keypad::initialize()
{
	initializeButtons();
	bringUpKeyboardTimer();
	sInstance = this;
}

void Keypad::initializeButtons()
{
	// Initialize 'forward' button btnForward;
	// set key id - as defined in hardwire description header
	btnForward.setKeyId(KEY_FORWARD);
	btnForward.setDirRegister(KEY_FORWARD_DIRECT_REGISTER);
	btnForward.setPortRegister(KEY_FORWARD_PORT_REGISTER);
	btnForward.setPinRegister(KEY_FORWARD_PIN_REGISTER);
	btnForward.setKeyPinNo(KEY_FORWARD_PIN);
	#ifdef KEY_FORWARD_HIGH_LEVEL_ON
	btnForward.setIsHightStateOn(true);
	#else
	btnForward.setIsHightStateOn(false);
	#endif
	btnForward.setButtonCallback(this);
	
	// Initialize 'back' button btnBack
	btnback.setKeyId(KEY_BACK);
	btnback.setDirRegister(KEY_BACK_DIRECT_REGISTER);
	btnback.setPortRegister(KEY_BACK_PORT_REGISTER);
	btnback.setPinRegister(KEY_BACK_PIN_REGISTER);
	btnback.setKeyPinNo(KEY_FORWARD_PIN);
	#ifdef KEY_BACK_HIGH_LEVEL_ON
	btnback.setIsHightStateOn(true);
	#else
	btnback.setIsHightStateOn(false);
	#endif
	btnback.setButtonCallback(this);
	
	// initialize button (hardware pins)
	btnForward.initDirectionDegister();
	btnback.initDirectionDegister();
}


/*
 Keyboard (forward and back buttons use separate timer for updated. I did that,
 because adding another one (the third) SequencePlayer would increase interrupt
 time, what, in turn, causes some flickering in RGB diode. Besides, there is 
 one more timer - Timer2. 
 Even though I know to timers work, this init code is taken from
 http://www.instructables.com/id/Arduino-Timer-Interrupts/step2/Structuring-Timer-Interrupts/
 */
void Keypad::bringUpKeyboardTimer(){
	cli();
	//set timer2 interrupt at 8kHz
	TCCR2A = 0;// set entire TCCR2A register to 0
	TCCR2B = 0;// same for TCCR2B
	TCNT2  = 0;//initialize counter value to 0
	// set compare match register for 8khz increments
	OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
	// turn on CTC mode
	TCCR2A |= (1 << WGM21);
	// Set CS21 bit for 8 prescaler
	TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);
	// enable timer compare interrupt
	TIMSK2 |= (1 << OCIE2A);
	sei(); // Enable global interrupts
}

void Keypad::setCallback(KeypadCallback* pKeypadCallback)
{
	pCallback = pKeypadCallback;
}

void Keypad::updateManually()
{
	
}

void Keypad::onPressStateChanged(uint8_t buttonId, bool isPressed)
{
	
}