/*
 * RGB_LED_Driver.c
 *
 * Created: 16.12.2015 13:11:27
 *  Author: Alex
 */ 
#include <interrupt.h>
#include <avr/sfr_defs.h>
#include "compiler.h"
#include "RGB_LED_Driver.h"
#include "../src/include/LogicAnalyzerDebug.h"
#include "../src/include/utility.h"

#define SHIFT_COLOR_TO_TARGET_IN_N_FRAMES 1

#ifdef USE_RGB_LED

bool is_pin_high_state;
uint8_t tick_count;
LEDColor led_color;
/** indicates whether user selected a new color and we're changing color to match it */
bool is_moving_towards_color;
LEDColor next_color;
int color_shift_frame_counter;


// forward declaration, private func, don't need in .h file
void process_pwm_tick(void);

void initRgbLedDriver(void){
	uint8_t ddr_mask = _BV(LED_PIN_CHANNEL_RED) |
	_BV(LED_PIN_CHANNEL_GREEN) |
	_BV(LED_PIN_CHANNEL_BLUE);
	*(LED_PORT_DIRECT) |= ddr_mask;
	*LED_PORT  |= ddr_mask;
	
	//initLED();
	//clear_bit(LED_PORT, LED_PIN_CHANNEL_BLUE);
	//clear_bit(LED_PORT, LED_PIN_CHANNEL_RED);
	
	// show nothing by default, use setColor() method
	led_color.red = 0;
	led_color.green = 0;
	led_color.blue = 0;
	next_color = led_color;
	is_moving_towards_color = false;
	color_shift_frame_counter = 0;
	
#ifdef DEBUG_WITH_LOGIC_ANALYER
	initAnalyzerLEDPins();
#endif
	initTimerForLedPWM();
}

void setColor(LEDColor* pColor)
{	
	led_color.red = pColor->red;
	led_color.green = pColor->green;
	led_color.blue = pColor->blue;
	/** don't show 'moving color' animation, if use explicitly asked to change color right now */
	is_moving_towards_color = false;
	next_color = *pColor;
	
	// show color explicitly and clear tick counter
	tick_count = 0;
}

void nextRandomColor(){
	next_color.red = (uint8_t)( random() * 1.0 / RAND_MAX * 255);
	next_color.green = (uint8_t)( random() * 1.0 / RAND_MAX * 255);
	next_color.blue = (uint8_t)( random() * 1.0 / RAND_MAX * 255);
	shiftToColor(&next_color);
}


void make_step_to_color(){
	bool changed = false;
	if (next_color.red != led_color.red){
		changed = true;
		if (next_color.red > led_color.red)
		led_color.red++;
		else
		led_color.red--;
	}
	if (next_color.green != led_color.green){
		changed = true;
		if (next_color.green > led_color.green)
		led_color.green++;
		else
		led_color.green--;
	}
	if (next_color.blue != led_color.blue){
		changed = true;
		if (next_color.blue > led_color.blue)
		led_color.blue++;
		else
		led_color.blue--;
	}
	if (changed)
	setColor(&next_color);
	else
	is_moving_towards_color = false;
}


void shiftToColor(LEDColor* pColor){
	next_color.red = pColor->red;
	next_color.green = pColor->green;
	next_color.blue = pColor->blue;
	is_moving_towards_color = true;
}


LEDColor* getNextColor(void) {
	return &next_color;
}

void delay(void){
	wait(100);
}

ISR(TIMER1_COMPB_vect)
{	
	if (++tick_count > 63){	
		tick_count = 0;
		if (is_moving_towards_color) {
			if (++color_shift_frame_counter == SHIFT_COLOR_TO_TARGET_IN_N_FRAMES){
				color_shift_frame_counter = 0;
				make_step_to_color();		
			}
		}
	}	
	process_pwm_tick();
	
	uint16_t dif = 0xffff - TCNT1;
	if (dif < RGB_CALLBACK_FIRE_RATE){
		OCR1B = RGB_CALLBACK_FIRE_RATE - dif;
	}else {
		OCR1B += RGB_CALLBACK_FIRE_RATE;
	}
	
		
}

void process_pwm_tick(void)
{
	if (tick_count == 0){
		// begin of a new pulse, turn all channels on
		if (led_color.red != 0){
			turnPinOn(LED_PORT, LED_PIN_CHANNEL_RED);
#ifdef DEBUG_WITH_LOGIC_ANALYER
			setAnalyzerPin(LOGIC_ANALYZER_PIN_R);
#endif
		}
		if (led_color.green != 0) {
			turnPinOn(LED_PORT, LED_PIN_CHANNEL_GREEN);
#ifdef DEBUG_WITH_LOGIC_ANALYER
			setAnalyzerPin(LOGIC_ANALYZER_PIN_G);
#endif
		}
		if (led_color.blue != 0){
			turnPinOn(LED_PORT, LED_PIN_CHANNEL_BLUE);
#ifdef DEBUG_WITH_LOGIC_ANALYER
			setAnalyzerPin(LOGIC_ANALYZER_PIN_B);
#endif
		}
	}else {
		uint8_t tick8bit = tick_count * 4;
		if (tick8bit >= led_color.red){
			turnPinOff(LED_PORT, LED_PIN_CHANNEL_RED);
#ifdef DEBUG_WITH_LOGIC_ANALYER
			clearAnalyzerPin(LOGIC_ANALYZER_PIN_R);
#endif
		}
		if (tick8bit >= led_color.green) {
			turnPinOff(LED_PORT, LED_PIN_CHANNEL_GREEN);
#ifdef DEBUG_WITH_LOGIC_ANALYER
			clearAnalyzerPin(LOGIC_ANALYZER_PIN_G);
#endif
		}
		if (tick8bit >= led_color.blue) {
			turnPinOff(LED_PORT, LED_PIN_CHANNEL_BLUE);
#ifdef DEBUG_WITH_LOGIC_ANALYER
			clearAnalyzerPin(LOGIC_ANALYZER_PIN_B);
#endif
		}
	}
}


void initTimerForLedPWM(void)
{
	// we're using compare register A for mono channels
	TIMSK1 =  (0 << TOIE1) | (1 << OCIE1B);
	TCCR1B =  (0 << CS12) | (1 << CS11) | (0 << CS10);

	is_pin_high_state = true;
	tick_count = 0;
	// fire every 100 uSec
	OCR1B = RGB_CALLBACK_FIRE_RATE;
	TCNT1 = 0;

	sei();
}

#endif
