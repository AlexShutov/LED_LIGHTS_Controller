/*
 * PortUtils.h
 *
 * Created: 17.12.2015 15:16:40
 *  Author: Alex
 */ 


#ifndef PORTUTILS_H_
#define PORTUTILS_H_
#include "compiler.h"

void set_bit(volatile uint8_t*, uint8_t);
void clear_bit(volatile uint8_t*, uint8_t);


void inline turnPinOn(volatile uint8_t* pPort, uint8_t pinNo){
	#ifdef LOW_LEVEL_IS_ON
	clear_bit(pPort, pinNo);
	#else
	set_bit(pPort, pinNo);
	#endif
}
void inline turnPinOff(volatile uint8_t* pPort, uint8_t pinNo){
	#ifdef LOW_LEVEL_IS_ON
	set_bit(pPort, pinNo);
	#else
	clear_bit(pPort, pinNo);
	#endif
}


#endif /* PORTUTILS_H_ */