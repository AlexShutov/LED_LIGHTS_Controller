/* 
* Button.h
*
* Created: 8/24/2016 11:31:31 PM
* Author: Alex
*/


#ifndef __BUTTON_H__
#define __BUTTON_H__


#include <avr/io.h>
#include "compiler.h"
#include "../src/comm/ManuallyUpdatable.h"

class ButtonPressCallback {
public:
	// fired when button state changed during last time interval
	virtual void onPressStateChanged(uint8_t buttonId, bool isPressed);
};

class Button : public ManuallyUpdatable
{
//variables
public:
protected:
private:
	volatile uint8_t* pDirRegister;
	volatile uint8_t* pPortRegister;
	volatile uint8_t* pPinRegister;
	uint8_t keyPinNo;
	bool isHighStateOn;
	// callback has to know what key it is dealing with
	uint8_t keyId;
	// callback, telling that button state has changed
	ButtonPressCallback* pButtonCallback;
	
	// if button was pressed during last update,
	// true - was pressed, false - was not
	bool lastPressState;
	
	
//functions
public:
	Button();	
	virtual ~Button();
	void setKeyId(uint8_t keyId);
	void setDirRegister(volatile uint8_t* pRegister);
	void setPortRegister(volatile uint8_t* pRegister);
	void setPinRegister(volatile uint8_t* pRegister);
	void setKeyPinNo(uint8_t keyPinNo);
	void setIsHightStateOn(bool isHighStateOn);
	
	void setButtonCallback(ButtonPressCallback* pCallback);
	// I didn't moved this method into ManuallyUpdatable base class, 
	// because manually updatable entity is usually updated from
	// inside by some interrupt callback. In this case 'Keyboard'
	// has few button and 'Keyboard' receive event from interrupt, not
	// button itself. 'Keyboard', in turn, update every button,
	// thus this method is here
	void checkStateFromInterrupt();
	
	virtual void updateManually();
	
	
	// Initialize pin assigned to this button
	// as input pin
	void initDirectionDegister();
	
	// check if button is pressed
	bool isButtonPressed();

	
protected:
private:
	Button( const Button &c );
	Button& operator=( const Button &c );
	
}; //Button



#endif //__BUTTON_H__
