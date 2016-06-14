/*
 * LogicAnalyzerDebug.c
 *
 * Created: 17.12.2015 14:46:27
 *  Author: Alex
 */ 
#include "../src/include/LogicAnalyzerDebug.h"
#include "../src/include/PortUtils.h"


void initAnalyzerOutPins(volatile uint16_t* pDirPort,
						 volatile uint8_t* pPort,
						 uint8_t pinMask) {
	*pDirPort |= pinMask;
	// set all pins to lower state
	*pPort &= ~pinMask;
}

void initAnalyzerLEDPins(){
	uint8_t mask = _BV(LOGIC_ANALYZER_PIN_R) |
				   _BV(LOGIC_ANALYZER_PIN_G) |
				   _BV(LOGIC_ANALYZER_PIN_B);
	initAnalyzerOutPins(LOGIC_ANALYZER_DIR_PORT, 
					LOGIC_ANALYZER_PORT, mask);	
}

void setAnalyzerPin(uint8_t pin){
	set_bit(LOGIC_ANALYZER_PORT, pin);	
}
void clearAnalyzerPin(uint8_t pinNo){
	clear_bit(LOGIC_ANALYZER_PORT, pinNo);
}
