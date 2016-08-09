/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/delay.h>
#include <avr/io.h>
#include "compiler.h"
#include "hardware.h"
#include "../src/Utils/operators.h"
#include "hardware_drivers/RGB_Led.h"
#include "hardware_drivers/Strobe.h"

#include "../src/comm/CommExecutorFacade.h"
#include "../src/timed_pulse/TimeIntervalGenerator.h"
#include "../src/timed_pulse/EventCallbackDecorator.h"
#include "../src/timed_sequence/SequencePlayer.h"
#include "../src/timed_pulse/EventCallbackCustomActions.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"

using TimeIntervalGeneration::TimeIntervalGenerator;
using TimeIntervalGeneration::EventCallbackDecorator;
using TimeIntervalGeneration::SequencePlayer;
using TimeIntervalGeneration::EventCallbackCustomActions;

using namespace LedCommandExecutors;
using namespace StrobeRelated;

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

void whiteColor(){
	Color c;
	Color::clear(&c);
	c.red = 255;
	c.green = 255;
	c.blue = 255;
	RGB_Led::setColor(&c);
}

class RedLightEndAction : public ExternalEndCallback
{
public:
	void onSequenceRestarted(uint8_t commandCode)
	{
		if (commandCode == COMMAND_CODE_LIGHT_SEQUENCE){
			ok();
		}
	}	
	
	void onSequenceEnded(uint8_t commandCode)
	{
		if (commandCode == COMMAND_CODE_LIGHT_SEQUENCE) {
			whiteColor();
		}
	}
};
RedLightEndAction redLightAction;


ColorSequenceExecutor seqExec;
StrobeLightsExecutor strobeExec;
char buff[200];
void testSequencePlayer(){
	seqExec.setCommandCode(COMMAND_CODE_LIGHT_SEQUENCE);
	seqExec.setSequencePlayer(&sp);
	seqExec.setExternalEndCallback(&redLightAction);
	
	CommColorHeader* pH = (CommColorHeader*) buff;
	pH->isSmoothSwitch= true;
	pH->numberOfLights = 3;
	pH->repeat = false;
	
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


void testStrobe(Strobe* pStrobe, SequencePlayer* pPlayer){
	strobeExec.setSequencePlayer(pPlayer);
	strobeExec.setStrobe(pStrobe);
	
	CommandStrobesDataHeader* pStrobeHeader = (CommandStrobesDataHeader*) buff;
	pStrobeHeader->isItPermanent = false;
	pStrobeHeader->isON = true;
	pStrobeHeader->numberOfFlashes = 3;
	pStrobeHeader->repeat = true;
	
	CommandStrobesDataRecord* pRec = (CommandStrobesDataRecord*)( pStrobeHeader + 1);
	TimeInterval* pT = &pRec->flashDuration;
	
	pT->milliseconds = 20;
	pT->seconds = 0;
	pT->minutes = 0;
	pT = &pRec->pauseDuration;
	pT->milliseconds = 70;
	pT->seconds = 0;
	pT->minutes = 0;
	pRec++;
	
	pT = &pRec->flashDuration;
	pT->milliseconds = 50;
	pT->seconds = 0;
	pT->minutes = 0;
	pT = &pRec->pauseDuration;
	pT->milliseconds = 100;
	pT->seconds = 0;
	pT->minutes = 0;
	pRec++;
	
	pT = &pRec->flashDuration;
	pT->milliseconds = 200;
	pT->seconds = 0;
	pT->minutes = 0;
	pT = &pRec->pauseDuration;
	pT->milliseconds = 0;
	pT->seconds = 1;
	pT->minutes = 0;
	
	IncomingCommand command;
	command.setCommandCode(COMMAND_STROBE_SEQUENCE);
	command.setDataBlockSize(sizeof(CommandStrobesDataHeader) + 
		3 * sizeof(CommandStrobesDataRecord));
	command.setBufferPtr(buff);
	strobeExec.executeCommand(&command);
}

int main(void)
{
	RGB_Led::init();
	uartInit();
	TimeIntervalGenerator::setupTimedPulse();
	sp.setPulseGeneratorIndex(1);
	
	testSequencePlayer();
	
	CommExecutorFacade facade;
	facade.initialize();
	
	//testStrobe(facade.getStrobe(), &sp);
	
	while (1) 
    {		
		facade.pollForCommand();
		//commandReceiver.receiveCommand();
    }
}

