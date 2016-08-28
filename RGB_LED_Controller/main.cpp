/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include "compiler.h"
#include "hardware.h"
#include "../src/Utils/operators.h"
#include "../src/comm/CommExecutorFacade.h"

#include "../src/hardware_drivers/RGB_Led.h"
#include "../src/hardware_drivers/keyboard/Button.h"

#include "../src/hardware_drivers/keyboard/Keypad.h"

using TimeIntervalGeneration::TimeIntervalGenerator;
using namespace TimeIntervalGeneration;

extern "C" {
	#include "../src/include/uart_stuff.h"
};

Color c;
void ok(){
	Color::clear(&c);
	c.green = 255;
	RGB_Led::setColor(&c);
}

void error(){
	Color::clear(&c);
	c.red = 255;
	RGB_Led::setColor(&c);
}

class KeypadColorCallback : public KeypadCallback
{
private:	
	Color color;
public:
	KeypadColorCallback(){
		Color::clear(&color);
	}	
	
	virtual void onBackButton(bool isPress){
		Color::clear(&color);
		if (isPress)
			color.red = 255;
		else
			color.red = 50;
		RGB_Led::setColor(&color);
	}
	
	virtual void onForwardButton(bool isPress){
		Color::clear(&color);
		if (isPress)
			color.green = 255;
		else
			color.green = 50;
		RGB_Led::setColor(&color);
	}
	
	virtual void onStartButtonPressed(){
		color.red = 255;
		color.green = 255;
		color.blue = 30;
		RGB_Led::setColor(&color);
	}
	
	virtual void onStopButtonPressed(){
		color.red = 30;
		color.green = 255;
		color.blue = 255;
		RGB_Led::setColor(&color);
	}
};

class PlayerKeypadCallback : public KeypadCallback{
private:
	EESupport::EEPlayer* pPlayer;
public:
	PlayerKeypadCallback(){
		pPlayer = 0;
	}
	void setPlayer(EESupport::EEPlayer* pEEPlayer){
		pPlayer = pEEPlayer;
	}
	
	virtual void onBackButton(bool isPress){
		if (isPress && pPlayer){
			pPlayer->back();
		}
	}
	virtual void onForwardButton(bool isPress){
		if (isPress && pPlayer){
			pPlayer->forward();
		}
	}
	virtual void onStartButtonPressed(){
		if (pPlayer){
			pPlayer->reloadCurrentCell();
		}
	}
	virtual void onStopButtonPressed(){
		if (pPlayer){
			pPlayer->stopBackgroundCommands();
		}
	}
};
Strobe testStrobe;
int main(void)
{
	// initialize hardware drivers
	RGB_Led::init();
	uartInit();
	TimeIntervalGenerator::setupTimedPulse();
	// create and initialize device facade (see
	// pattern description in GOF)
	CommExecutorFacade facade;
	facade.initialize();
	// reload command saved as current in EEPROM
	facade.getEEPlayer()->reloadCurrentCell();
	
	Keypad keypad;
	//KeypadColorCallback keypadCallback;
	PlayerKeypadCallback keypadPlayback;
	keypadPlayback.setPlayer(facade.getEEPlayer());
	keypad.setPlaybackState(true);
	keypad.setCallback(&keypadPlayback);
	keypad.initialize();
	
	
	while (1) 
    {		
			
		facade.pollForCommand();
		facade.updateManually();
		keypad.updateManually();
		
	}
}

