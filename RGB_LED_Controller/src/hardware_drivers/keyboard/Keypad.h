/* 
* Keypad.h
*
* Created: 8/27/2016 5:34:24 PM
* Author: Alex
*/


#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "compiler.h"
#include "Button.h"
#include "../src/comm/ManuallyUpdatable.h"
#include "../src/hardware_drivers/RGB_Led.h"


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
 
class KeypadCallback
{
public:
	// 'back' button
	virtual void onBackButton(bool isPress) = 0;
	// 'forward' button
	virtual void onForwardButton(bool isPress) = 0;
	// Those are software buttons - including 'release' 
	// will complicate logic. Actually, we don't need 'released' state
	// 'start' button - state of two button press
	virtual void onStartButtonPressed() = 0;
	// 'stop' button
	virtual void onStopButtonPressed() = 0;
};

class Keypad : protected ButtonPressCallback
{
//variables
public:
	// used for calling methods from timer's interrupt
	static Keypad* sInstance;
protected:
private:
	public:

	// buttons, assigned to particular pins
	Button btnForward;
	Button btnback;
	// callback set by the rest of program
	KeypadCallback* pCallback;
	
	// flags set inside button callback and indicating
	// if state of button have changed
	bool btnBackChanged;
	bool btnForwardChanged;
	// we need current state for handling situation when
	// both buttons is pressed simultaneously - it can
	// mean 'stop' or 'play'
	bool isNowPlaying;
	
	// Button's callbasks
	virtual void onPressStateChanged(uint8_t buttonId, bool isPressed);

//functions
public:
	Keypad();
	~Keypad();
	void initialize();
	void setCallback(KeypadCallback* pKeypadCallback);
	void setPlaybackState(bool isPlaying);
	// program calls this method from main loop - we can make long 
	// operations here
	void updateManually();
	void checkStateFromInterrupt();
	
protected:
	
private:
Color color;
	Keypad( const Keypad &c );
	Keypad& operator=( const Keypad &c );
	// assign buttons to pins as defined in 'hardware.h' header
	void initializeButtons();
	
	void bringUpKeyboardTimer();
	// It is finally called when state of all buttons is updated (
	// last call from 'updateManually()' method. State of buttons is 
	// analyzed here and callback notified if needed
	void processNewData();
	// Player can be stopped or can be play sequence right now. 
	void hadleTwoButtonsPress();
}; //Keypad

#endif //__KEYPAD_H__

