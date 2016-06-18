/*
 * uart_stuff.c
 *
 * Created: 22.12.2015 2:57:51
 *  Author: Alex
 */ 
#include "../src/include/uart_stuff.h"
#include "../src/UART/uart.h"



// Only if enabled
#ifdef UART_ENABLED


void uartInit(void) {
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
}

/** Write a single character
 *
 * Send a single character on the UART.
 *
 * @param ch the character to send.
 */
void uartSend(char ch) {
	uart_putc((unsigned char) ch);
}



/** Determine if characters are available
 *
 * Check the number of characters available for immediate reading. If this
 * function returns a non-zero value the next call to uartRecv() will be
 * guaranteed to return immediately with a value. This function is only valid
 * if you are using the interrupt driven version of the UART.
 *
 * @return the number of characters available in the input buffer.
 */
uint8_t uartAvail() {
	return uart0_available();
}

/** Receive a single character
 *
 * Wait for a single character on the UART and return it.
 *
 * @return the character received.
 */
char uartRecv() {
	return uart0_getc();
 }

#endif /* UART_ENABLED */

