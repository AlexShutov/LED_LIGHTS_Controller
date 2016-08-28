/* 
* Keypad.h
*
* Created: 8/27/2016 5:34:24 PM
* Author: Alex
*/


#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "Button.h"

/************************************************************************/
/*		'Back' and 'Forward' buttons for selecting sequnce from memory 
/* Device is assembled into mouse case, which has only two buttons, but 
/* we have to define 'stop' and 'start' actions. 
/* Pressing both keys simultaneously will trigger 'start' or 'stop' 
/* action, depending on previous state.
/* Notice, there is single keypad, but it is not implemented as
/* 'Singleton' due to avr specifics. Nevertheless, KeypadCallback has pointer
/* to instance
/************************************************************************/
 
abstract class KeypadCallback
{
	virtual void onBackPressed();
	virtual void onForwardPressed();
	virtual void onStartPressed();
	virtual void onStopPressed();	
};

class Keypad : protected ButtonPressCallback
{
//variables
public:
// used for calling methods from timer's interrupt
	static Keypad* sInstance;
protected:
private:
	// buttons, assigned to particular pins
	Button btnForward;
	Button btnback;
	// callback set by the rest of program
	KeypadCallback* pCallback;
	// key codes for distinguishing buttons inside callback
	uint8_t btnForwardCode;
	uint8_t btnBackCode;
	
// Button's callbasks
virtual void onPressStateChanged(uint8_t buttonId, bool isPressed);

//functions
public:
	Keypad();
	~Keypad();
	void initialize();
	void setCallback(KeypadCallback* pKeypadCallback);
	void updateManually();
	
protected:
	
private:
	Keypad( const Keypad &c );
	Keypad& operator=( const Keypad &c );
	// assign buttons to pins as defined in 'hardware.h' header
	void initializeButtons();
	
	void bringUpKeyboardTimer();
}; //Keypad

#endif //__KEYPAD_H__

