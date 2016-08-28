/* 
* Button.cpp
*
* Created: 8/24/2016 11:31:31 PM
* Author: Alex
*/

#include "Button.h"
#include "../src/hardware_drivers/RGB_Led.h"


// default constructor
Button::Button()
:ManuallyUpdatable()
{
	pButtonCallback = 0;
	// set as manually updatable by default
	lastPressState = false;
	// always update manually, not from interrupt, 
	// because interrupt callbacks has to be as short
	// as possible
	setManualUpdate(true);
	markAsNotUpdated();
} //Button

// default destructor
Button::~Button()
{
	
} //~Button

void Button::setKeyId(uint8_t keyId)
{
	this->keyId = keyId;
}

void Button::setDirRegister(volatile uint8_t* pRegister)
{
	this->pDirRegister = pRegister;
}

void Button::setPortRegister(volatile uint8_t* pRegister)
{
	this->pPortRegister = pRegister;
}

void Button::setPinRegister(volatile uint8_t* pRegister)
{
	this->pPinRegister = pRegister;
}

void Button::setKeyPinNo(uint8_t keyPinNo)
{
	this->keyPinNo = keyPinNo;
}

void Button::setIsHightStateOn(bool isHighStateOn)
{
	this->isHighStateOn = isHighStateOn;
}

void Button::setButtonCallback(ButtonPressCallback* pCallback){
	pButtonCallback = pCallback;
}


void Button::updateManually()
{
	if (!isUpdateNeeded()){
		// button state didn't change
		return;
	}
	if (!pButtonCallback){
		// reset state
		markAsUpdated();
		// and quit
		return;
	}
	bool isPressed = false;
	if (isHighStateOn ^ lastPressState ){
		isPressed = true;
	}else {
		isPressed = false;
	}
	pButtonCallback->onPressStateChanged(keyId, isPressed);
	
	markAsUpdated();
}

void Button::checkStateFromInterrupt()
{
	bool isNowPressed = isButtonPressed();
	if (isNowPressed == lastPressState){
		// button state not changed, do nothing
		return;
	}
	
	// button was pressed during last update interval
	// set current press state as last state
	lastPressState = isNowPressed;
	// mark button as needed manual update
	markAsNotUpdated();
}


void Button::initDirectionDegister()
{
	// for input mode we clear pin in direction register	
	*pDirRegister &= ~_BV(keyPinNo);
	// and activate pull-up resistor by setting pin in
	// port register
	*pPortRegister |= _BV(keyPinNo);
}

bool Button::isButtonPressed()
{
	// read bit from pin register
	bool isPinInHighState = *pPinRegister & _BV(keyPinNo);
	return isHighStateOn ? isPinInHighState : !isPinInHighState;
}