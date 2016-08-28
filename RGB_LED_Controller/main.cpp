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

#include "../src/comm/ManuallyUpdatable.h"
#include "../src/timed_sequence/SequencePlayer.h"
#include <interrupt.h>

using TimeIntervalGeneration::TimeIntervalGenerator;
using namespace TimeIntervalGeneration;

extern "C" {
	#include "../src/include/uart_stuff.h"
};

class BtnPressCallback : public ButtonPressCallback
{
public:
	void onPressStateChanged(uint8_t buttonId, bool isPressed){
		
		Color c;
		Color::clear(&c);
		if (isPressed){
			c.green = 255;
		}else {
			c.red = 255;
		}
		RGB_Led::setColor(&c);
	}	
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



Button btnForward;

Strobe testStrobe;
ISR(TIMER2_COMPA_vect)
{
	if (testStrobe.isOn()){
		testStrobe.turnOff();
	}else {
		testStrobe.turnOn();
	}
	btnForward.checkStateFromInterrupt();
	
}

/*
 Keyboard (forward and back buttons use separate timer for updated. I did that,
 because adding another one (the third) SequencePlayer would increase interrupt
 time, what, in turn, causes some flickering in RGB diode. Besides, there is 
 one more timer - Timer2. 
 Even though I know to timers work, this init code is taken from
 http://www.instructables.com/id/Arduino-Timer-Interrupts/step2/Structuring-Timer-Interrupts/
 */
void bringUpKeyboardTimer(){
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
	
	TimeIntervalGeneration::TimeIntervalGenerator pulseGenerator;
	BtnPressCallback callback;
	
	//Button btnForward;
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
	btnForward.setButtonCallback(&callback);
	// initialize button (hardware)
	btnForward.initDirectionDegister();
	
	//Strobe testStrobe;
	testStrobe.initStrobe(&DDRB, &PORTB, 1, true);
	
	bringUpKeyboardTimer();
	
	while (1) 
    {		
			
		facade.pollForCommand();
		facade.updateManually();
		/*btnForward.checkStateFromInterrupt();
		btnForward.updateManually();
		_delay_ms(100);
		*/
		btnForward.updateManually();
		
	}
}

