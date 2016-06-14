/*
 * LogicAnalyzerDebug.h
 *
 * Created: 17.12.2015 14:46:13
 *  Author: Alex
 */ 


#ifndef LOGICANALYZERDEBUG_H_
#define LOGICANALYZERDEBUG_H_
#include "compiler.h"

#define DEBUG_WITH_LOGIC_ANALYER

#define LOGIC_ANALYZER_DIR_PORT &DDRD
#define LOGIC_ANALYZER_PORT &PORTD
#define LOGIC_ANALYZER_PIN_0 0
#define LOGIC_ANALYZER_PIN_1 1
#define LOGIC_ANALYZER_PIN_2 2
#define LOGIC_ANALYZER_PIN_3 3
#define LOGIC_ANALYZER_PIN_4 4
#define LOGIC_ANALYZER_PIN_5 5

#define LOGIC_ANALYZER_PIN_R LOGIC_ANALYZER_PIN_0
#define LOGIC_ANALYZER_PIN_G LOGIC_ANALYZER_PIN_1
#define LOGIC_ANALYZER_PIN_B LOGIC_ANALYZER_PIN_4

void initAnalyzerOutPins(volatile uint16_t* pDirPort,
						 volatile uint8_t* pPort,
						 uint8_t pinMask);
						 
void initAnalyzerLEDPins(void);
void setAnalyzerPin(uint8_t pin);
void clearAnalyzerPin(uint8_t pinNo);
#endif /* LOGICANALYZERDEBUG_H_ */