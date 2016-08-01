/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/delay.h>
#include <avr/io.h>
#include "compiler.h"
#include "../src/Utils/operators.h"
#include "hardware_drivers/RGB_Led.h"

#include "../src/comm/CommExecutorFacade.h"
#include "../src/timed_pulse/TimeIntervalGenerator.h"
#include "../src/timed_pulse/EventCallbackDecorator.h"
#include "../src/timed_sequence/SequencePlayer.h"
#include "../src/timed_pulse/EventCallbackCustomActions.h"

using TimeIntervalGeneration::TimeIntervalGenerator;
using TimeIntervalGeneration::EventCallbackDecorator;
using TimeIntervalGeneration::SequencePlayer;
using TimeIntervalGeneration::EventCallbackCustomActions;

using namespace LedCommandExecutors;

extern "C" {
	#include "../src/include/uart_stuff.h"
};

Color c;
void f1(uint8_t pulseIndex){
	c.red = 255;
	c.green = 0;
	c.blue = 0;
	RGB_Led::setColor(&c);
}
void f2(uint8_t pulseIndex){
	c.red = 0;
	c.green = 255;
	c.blue = 0;
	RGB_Led::setColor(&c);
}


Color cols[3];
ColorCallback colorCallback;
EventCallbackCustomActions customAction;
TimeInterval durs[3];
SequencePlayer sp;
SequenceTerminateCallback terminate;


void testSequence(){
	
	Color* pc = &cols[0];
	pc->red = 255;
	pc->green = 0;
	pc->blue = 0;
	pc++;
	
	pc->red = 0;
	pc->green = 255;
	pc->blue = 0;
	pc++;
	
	pc->red = 0;
	pc->green = 0;
	pc->blue = 255;
	
	colorCallback.setColor(cols, 3);
	
	//customAction.setCustomActions(colorCallback, 3);
	customAction.setCustomActionToMany(&colorCallback, MAX_CUSTOM_ACTIONS);
	//customAction.setCustomActions(&colorCallback, MAX_CUSTOM_ACTIONS);
	TimeInterval* pt = durs;
	pt->milliseconds = 100;
	pt->seconds = 0;
	pt->minutes = 0;
	pt++;
	
	pt->milliseconds = 100;
	pt->seconds = 0;
	pt->minutes = 0;
	pt++;
	
	pt->milliseconds = 100;
	pt->seconds = 0;
	pt->minutes = 0;
	
	sp.setPulseGeneratorIndex(0);
	sp.init();
	sp.setIntervalEndCallback(&customAction);
	sp.setTerminationCallback(&terminate);
	sp.setupSequence(durs, 3, true);
}

void error(){
	Color c;
	Color::clear(&c);
	c.red = 255;
	RGB_Led::setColor(&c);
}
void ok(){
	Color c;
	Color::clear(&c);
	c.green = 255;
	RGB_Led::setColor(&c);
}

void assert(char val, char needed){
	if (val == needed){
		ok();
	} else {
		error();
	}
}

ColorSequenceExecutor seqExec;
char buff[200];
void testSequencePlayer(){
	
	seqExec.setSequencePlayer(&sp);
	
	CommColorHeader* pH = (CommColorHeader*) buff;
	pH->isSmoothSwitch= true;
	pH->numberOfLights = 3;
	pH->repeat = true;
	
	CommColorSequenceRecord* pRec = (CommColorSequenceRecord*)( pH + 1);
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 20;
	pRec->pulseColor.blue = 0;
	pRec->pulseDuration.milliseconds = 300;
	pRec->pulseDuration.seconds = 0;
	pRec->pulseDuration.minutes = 0;
	pRec++;
	pRec->pulseColor.red = 0;
	pRec->pulseColor.green = 20;
	pRec->pulseColor.blue = 255;
	pRec->pulseDuration.milliseconds = 300;
	pRec->pulseDuration.seconds = 0;
	pRec->pulseDuration.minutes = 0;
	pRec++;
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 255;
	pRec->pulseColor.blue = 255;
	pRec->pulseDuration.milliseconds = 300;
	pRec->pulseDuration.seconds = 0;
	pRec->pulseDuration.minutes = 0;
	
	
	IncomingCommand command;
	command.setCommandCode(COMMAND_CODE_LIGHT_SEQUENCE);
	command.setDataBlockSize(sizeof(CommColorHeader) + 2 * sizeof(CommColorSequenceRecord));
	command.setBufferPtr(buff);
	seqExec.executeCommand(&command);
	
}

int main(void)
{
	RGB_Led::init();
	uartInit();
	TimeIntervalGenerator::setupTimedPulse();
	sp.setPulseGeneratorIndex(1);
	
	//testSequencePlayer();
	
	CommExecutorFacade facade;
	facade.initialize();
	
	
	while (1) 
    {		
		facade.pollForCommand();
		//commandReceiver.receiveCommand();
    }
}

