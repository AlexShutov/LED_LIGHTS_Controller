/*
 * TimeInterval.h
 *
 * Created: 6/19/2016 8:24:54 PM
 *  Author: Alex
 */ 


#ifndef TIMEINTERVAL_H_
#define TIMEINTERVAL_H_
#include <avr/io.h>

typedef struct {
	uint16_t milliseconds;
	uint8_t seconds;
	uint8_t minutes;
}TimeInterval;




#endif /* TIMEINTERVAL_H_ */