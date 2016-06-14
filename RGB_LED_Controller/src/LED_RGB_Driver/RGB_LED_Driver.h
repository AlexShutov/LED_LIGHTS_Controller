/*
 * RGB_LED_Driver.h
 *
 * Created: 16.12.2015 13:10:55
 *  Author: Alex
 */ 


#ifndef RGB_LED_DRIVER_H_
#define RGB_LED_DRIVER_H_
#include <avr/io.h>
#include "hardware.h"
#include "../src/include/PortUtils.h"


// comment for disabling support for specific type of led
#define USE_RGB_LED
#define USE_MONO_CHANNEL
#define NUMBER_OF_MONO_CHANNELS 5

#define RGB_CALLBACK_FIRE_RATE 0x64


#ifdef USE_RGB_LED


typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} LEDColor;


static volatile uint8_t* const LED_PORT_DIRECT = &DDRB;
static volatile uint8_t* const LED_PORT = &PORTB;
/*
static uint8_t const LED_PIN_CHANNEL_RED =  1;
static uint8_t const LED_PIN_CHANNEL_GREEN = 3;
static uint8_t const LED_PIN_CHANNEL_BLUE = 2;
*/
static uint8_t const LED_PIN_CHANNEL_RED =  2;
static uint8_t const LED_PIN_CHANNEL_GREEN = 4;
static uint8_t const LED_PIN_CHANNEL_BLUE = 3;

void delay(void);
void initRgbLedDriver(void);

void setColor(LEDColor* pColor);
void shiftToColor(LEDColor* pColor);
void nextRandomColor(void);
LEDColor* getNextColor(void);


#endif

void initTimerForLedPWM(void);

#endif /* RGB_LED_DRIVER_H_ */