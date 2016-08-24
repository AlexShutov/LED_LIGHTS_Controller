/*
 * RGB_LED_Controller.cpp
 *
 * Created: 6/13/2016 2:32:57 PM
 * Author : Alex
 */ 

#include <avr/delay.h>
#include <avr/io.h>

#include <avr/eeprom.h>

#include "compiler.h"
#include "hardware.h"
#include "../src/Utils/operators.h"
#include "hardware_drivers/RGB_Led.h"
#include "hardware_drivers/Strobe.h"
#include "../src/utils/Log.h"

#include "../src/comm/CommExecutorFacade.h"
#include "../src/timed_pulse/TimeIntervalGenerator.h"
#include "../src/timed_pulse/EventCallbackDecorator.h"
#include "../src/timed_sequence/SequencePlayer.h"
#include "../src/timed_pulse/EventCallbackCustomActions.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"
#include "../src/comm/PausedCommandDecorator.h"

#include "../src/EEManager/EEManager.h"
#include "../src/comm/comm_execs/EE/EEPlayer.h"

using TimeIntervalGeneration::TimeIntervalGenerator;
using TimeIntervalGeneration::EventCallbackDecorator;
using TimeIntervalGeneration::SequencePlayer;
using TimeIntervalGeneration::EventCallbackCustomActions;

using namespace LedCommandExecutors;
using namespace StrobeRelated;
using namespace EESupport;

extern "C" {
	#include "../src/include/uart_stuff.h"
};

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


void whiteColor(){
	Color c;
	Color::clear(&c);
	c.red = 255;
	c.green = 255;
	c.blue = 255;
	RGB_Led::setColor(&c);
}



#define EEBUFFER_SIZE 200
#define EE_OFFSET 100



void testEEProm(EEManager* eeManager)
{
	Color c;
	Color::clear(&c);
	c.red = 245;
	c.green = 0;
	c.blue = 10;
	
	//eeManager->writeData(1, 10, &c, sizeof(Color));
	Color::clear(&c);
	
	eeManager->readData(1, 10, &c, sizeof(Color));
	RGB_Led::setColor(&c);
}

char buff[200];
IncomingCommand cmd;
void testSavingCommandToEEColor(CommExecutorFacade* pFacade, 
								Color* pC,
								uint8_t cellNo,
								uint8_t cellOffset){
	EEPlayer* pPlayer = pFacade->getEEPlayer();
	// allocate command header at buffer start
	IncomingCommand* pCommand = (IncomingCommand*) buff;
	pCommand->setCommandCode(COMMAND_CODE_CHANGE_COLOR);
	pCommand->setDataBlockSize(sizeof(Color));
	// write data block right after command header
	char* pDataPtr = (char*)(pCommand + 1);
	pCommand->setBufferPtr(pDataPtr);
	// set color
	Color* pColor = (Color*) pDataPtr;
	/*
	pColor->red = 255;
	pColor->green = 255;
	pColor->blue = 0;
	*/
	*pColor = *pC;
	// save command with player (it will play that command after)
	pPlayer->saveToCell(pCommand, cellNo, cellOffset);
}

void testSavingCommandToEESequence(CommExecutorFacade* pFacade, 
								   uint8_t cellNo,
								   uint8_t cellOffset){
	EEPlayer* pPlayer = pFacade->getEEPlayer();
	// allocate command header at buffer start
	IncomingCommand* pCommand = (IncomingCommand*) buff;
	pCommand->setCommandCode(COMMAND_CODE_LIGHT_SEQUENCE);
//	pCommand->setDataBlockSize(sizeof(Color));
	// write data block right after command header
	char* pDataPtr = (char*)(pCommand + 1);
	pCommand->setBufferPtr(pDataPtr);
	// form data
	// fill in command header
	CommColorHeader* pHead = (CommColorHeader*) pDataPtr;
	pHead->isSmoothSwitch = false;
	pHead->numberOfLights = 2;
	pHead->repeat = true;
	CommColorSequenceRecord* pRec = (CommColorSequenceRecord*) (pHead + 1);
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 255;
	pRec->pulseDuration.milliseconds = 100;
	pRec->pulseDuration.minutes = 0;
	pRec->pulseDuration.seconds = 0;
	// move to the next color
	pRec++;
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.blue =255;
	pRec->pulseDuration.milliseconds = 100;
	pRec->pulseDuration.seconds = 0;
	pRec->pulseDuration.minutes = 0;
	
	uint8_t size = sizeof(CommColorHeader) + 2 * sizeof(CommColorSequenceRecord);
	pCommand->setDataBlockSize(size);
	
	// save command with player (it will play that command after)
	pPlayer->saveToCell(pCommand, cellNo, cellOffset);
}

void testLoadingCommandFromEE(CommExecutorFacade* pFacade,
							  uint8_t cellNo,
							  uint8_t cellOffset){
	EEPlayer* pPlayer = pFacade->getEEPlayer();
	pPlayer->loadFromCell(cellNo, cellOffset);
}
Color c;
int main(void)
{
	RGB_Led::init();
	uartInit();
	TimeIntervalGenerator::setupTimedPulse();
	
	CommExecutorFacade facade;
	
	facade.initialize();
	
	//testEEProm(facade.getEEManager());
	/*
	EEPlayer* pPlayer = facade.getEEPlayer();
	pPlayer->loadPlayerDataFromEEPROM();
	PlayerData* pPlayerData = pPlayer->getPlayerData();
	
	pPlayerData->savedPatternsInfo[3].isInUse = true;
	pPlayerData->savedPatternsInfo[5].isInUse = true;
	
	if (pPlayer->getNumberOfCellsInUse() == 2 &&
		pPlayer->isCellInUse(3)	 &&
		pPlayer->isCellInUse(5)){
	} else {
		error();
	}
	pPlayer->wipeOutPlayerData();
	
	pPlayerData->savedPatternsInfo[1].isInUse = true;
	pPlayerData->savedPatternsInfo[3].isInUse = true;
	pPlayerData->savedPatternsInfo[4].isInUse = true;
	
	pPlayerData->currentCellIndex = 6;
	pPlayer->back();
	if (pPlayerData->currentCellIndex == 4){
		ok();
	} else {
		error();
	}
	*/
	EEPlayer* pPlayer = facade.getEEPlayer();
	pPlayer->wipeOutPlayerData();
	/*
	testSavingCommandToEEColor(&facade, 0, 0);
	testSavingCommandToEESequence(&facade, 1, 0);
	*/
	Color::clear(&c);
	c.red = 255;
	testSavingCommandToEEColor(&facade, &c, 0, 0);
	Color::clear(&c);
	c.green = 255;
	testSavingCommandToEEColor(&facade, &c, 1, 0);
	Color::clear(&c);
	c.blue = 255;
	testSavingCommandToEEColor(&facade, &c,  2, 0);
	Color::clear(&c);
	c.red = 255;
	c.blue = 255;
	testSavingCommandToEEColor(&facade, &c, 3, 0);
	Color::clear(&c);
	c.red = 255;
	c.green = 255;
	c.blue = 255;
	testSavingCommandToEEColor(&facade, &c, 4, 0);
	
	testSavingCommandToEESequence(&facade, 5, 0);
	
	pPlayer->moveToCell(0);
	pPlayer->back();
	pPlayer->forward();
	pPlayer->back();
	
	
	
	//pPlayer->wipeOutPlayerData();
	while (1) 
    {		
		facade.pollForCommand();
		facade.updateManually();
		//commandReceiver.receiveCommand();
    }
}

