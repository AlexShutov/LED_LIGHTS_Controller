/*--------------------------------------------------------------------------*
* Hardware definitions
*---------------------------------------------------------------------------*
* 14-Apr-2014 ShaneG
*
* Defines the hardware pins to use for parts of the shared library.
*--------------------------------------------------------------------------*/
#ifndef __HARDWARE_H
#define __HARDWARE_H

// Bring in what we need for pin definitions
#include <avr/io.h>

//---------------------------------------------------------------------------
// Feature selection
//
// These defines control what code gets included in the library, make sure
// everything you want is uncommented. Further down the file you get to
// configure the pin assignments and other options for each feature.
//---------------------------------------------------------------------------

#define F_CPU 16000000

#define UART_ENABLED

//---------------------------------------------------------------------------
// Software UART configuration
//---------------------------------------------------------------------------


// Initially i've been using debug board with RGB LED, which is 'on' if pin is in
// low state, however, now I use LED strip controlled by MOSFETS. Those are open
// when pin is in a low state, that's why it is commented
//#define LOW_LEVEL_IS_ON 1

/** Baud rate to use
 *
 * The implementation is optimised for higher baudrates - please don't use
 * anything below 57600 on an 8MHz clock. It does work at up to 250000 baud
 * but you may experience a small amount of dropped packets at that speed.
 */
#define UART_BAUD_RATE 9600

// define register and pins to which RGB Led is connected to
#define LED_REGISTER_DIRECT  &DDRB
#define LED_REGISTER &PORTB
#define LED_PIN_RED 2
#define LED_PIN_GREEN 4
#define LED_PIN_BLUE 3

// define port and pin which strobe is connected to
#define FLASH_CHANNEL_DIRECTION_PORT &DDRB
#define FLASH_CHANNEL_PORT &PORTB
#define FLASH_CHANNEL_PIN_NO 5
//#define FLASH_CHANNEL_LOW_STATE_IS_ENABLED

// definitions for ee player's keyboard
// key back registers definition
// key ID's - for distinguising keys from callback
#define KEY_BACK 1
#define KEY_FORWARD 2
#define KEY_BACK_DIRECT_REGISTER &DDRC
#define KEY_BACK_PORT_REGISTER &PORTC
#define KEY_BACK_PIN_REGISTER &PINC
#define KEY_BACK_IS_ADC_PIN
// key forward registers definition
#define KEY_FORWARD_DIRECT_REGISTER &DDRC
#define KEY_FORWARD_PORT_REGISTER &PORTC
#define KEY_FORWARD_PIN_REGISTER &PINC
#define KEY_FORWARD_IS_ADC_PIN
// number of pins for player's keyboard
#define KEY_BACK_PIN 0
#define KEY_FORWARD_PIN 1
// whether key is pressed if pin in high level
// here buttons is activated by low level
//#define KEY_BACK_HIGH_LEVEL_ON
//#define KEY_FORWARD_HIGH_LEVEL_ON

/** Enable interrupt driven mode
 *
 * Only valid in two pin configuration. If this is defined and you are using
 * two pins for the UART then received data will be read and buffered as it
 * arrives.
 */
#define UART_INTERRUPT

/** Size of input buffer
 *
 * Only available in interrupt driven mode. This sets the size of the receive
 * buffer (max 256 bytes).
 */
#define UART_BUFFER 4


#endif /* __HARDWARE_H */
