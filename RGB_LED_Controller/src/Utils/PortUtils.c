/*
 * PortUtils.c
 *
 * Created: 17.12.2015 15:16:53
 *  Author: Alex
 */ 
#include "../src/include/PortUtils.h"

void set_bit(volatile uint8_t* pPort, uint8_t bit_pos){
	*pPort |= _BV(bit_pos);
}
void clear_bit(volatile uint8_t* pPort, uint8_t bit_pos){
	*pPort &= ~_BV(bit_pos);
}