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
#include "../src/comm/comm_execs/EECommandExecutor.h"

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


char blinkBuffer[100];
IncomingCommand* pBlingCommand;

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
	//pPlayer->saveToCell(pCommand, cellNo, cellOffset);
	pPlayer->saveToCell(cellNo, true, pCommand, pBlingCommand);
}

void testSavingCommandToEESequence(CommExecutorFacade* pFacade, 
								   uint8_t cellNo,
								   uint8_t cellOffset,
								   bool writeToEE){
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
	//pPlayer->saveToCell(pCommand, cellNo, cellOffset);
	if (writeToEE){
		pPlayer->saveToCell(cellNo, false, pCommand, 0);
	}
}


void initBlinkData(bool isOn){
	pBlingCommand = (IncomingCommand*) blinkBuffer;
	pBlingCommand->setCommandCode(COMMAND_STROBE_SEQUENCE);
	char* pDataBlock = (char*) (pBlingCommand + 1);
	pBlingCommand->setBufferPtr(pDataBlock);
	CommandStrobesDataHeader* pHeader = (CommandStrobesDataHeader*) pDataBlock;
	pHeader->isItPermanent = false;
	pHeader->isON = isOn;
	pHeader->numberOfFlashes = 1;
	pHeader->repeat = true;
	
	pBlingCommand->setDataBlockSize(sizeof(CommandStrobesDataHeader) + 
		 sizeof(CommandStrobesDataRecord));
	CommandStrobesDataRecord* pRec = (CommandStrobesDataRecord*)(pHeader + 1);
	pRec->flashDuration.milliseconds = 50;
	pRec->flashDuration.seconds = 0;
	pRec->flashDuration.minutes = 0;
	pRec->pauseDuration.milliseconds = 50;
	pRec->pauseDuration.seconds = 0;
	pRec->pauseDuration.minutes = 0;
	
}

void testEECommand(CommExecutorFacade* pFacade){
	
	// executor with history support
	CommandExecutor* pExec = pFacade->getExec();
	// test loading command from EEPROM
	IncomingCommand* pCommand = (IncomingCommand*) buff;
	pCommand->setCommandCode(COMMAND_EE);
	pCommand->setDataBlockSize(sizeof(EECommandData));
	EECommandData* pData = (EECommandData*) (pCommand + 1);
	pCommand->setBufferPtr((char*) pData);
	
	pData->isLoadCommand = true;
	pData->cellIndex = 5;
	pData->hasBackgroundCommand = false;
	
	pExec->executeCommand(pCommand);	
}

void testSavingCommandExec(CommExecutorFacade* pFacade){
	CommandExecutor* pExec = pFacade->getExec();
	IncomingCommand* pSaveCmd = (IncomingCommand*) buff;
	char* dataBlock = (char*) (pSaveCmd + 1);
	pSaveCmd->setCommandCode(COMMAND_EE);
	pSaveCmd->setBufferPtr(dataBlock);
	// set command size in the end
	EECommandData* pEECommData = (EECommandData*) dataBlock;
	pEECommData->cellIndex = 0;
	pEECommData->eraseCell = false;
	pEECommData->isLoadCommand = false;
	// test 'true' case later on
	pEECommData->hasBackgroundCommand = true;
	char* pData = (char*)(pEECommData + 1);
	
	// write 'change color' command
	IncomingCommand* pChangeColorCmd = (IncomingCommand*) pData;
	pChangeColorCmd->setCommandCode(COMMAND_CODE_CHANGE_COLOR);
	pChangeColorCmd->setDataBlockSize(sizeof(Color));
	char* pChangeColorData = (char*) (pChangeColorCmd + 1);
	pChangeColorCmd->setBufferPtr(pChangeColorData);
	Color* pColor = (Color*) pChangeColorData;
	// set Color
	pColor->red = 245;
	pColor->green = 245;
	pColor->blue = 22;
	uint8_t firstSize = sizeof(IncomingCommand) + sizeof(Color);
	
	// test if 'change color' command formed correctly
	//pExec->executeCommand(pChangeColorCmd);
	// it work, command formed correctly
	//pExec->executeCommand(pSaveCmd);
	
	// create another command- will be saved as background command
	// for that use strobe command
	
	IncomingCommand* pBckgCmd = (IncomingCommand*)(pColor + 1);
	pBckgCmd->setCommandCode(COMMAND_STROBE_SEQUENCE);
	char* pDataBlock = (char*) (pBckgCmd + 1);
	pBckgCmd->setBufferPtr(pDataBlock);
	CommandStrobesDataHeader* pHeader = (CommandStrobesDataHeader*) pDataBlock;
	pHeader->isItPermanent = false;
	pHeader->isON = false;
	pHeader->numberOfFlashes = 3;
	pHeader->repeat = true;
	
	CommandStrobesDataRecord* pRec = (CommandStrobesDataRecord*)(pHeader + 1);
	pRec->flashDuration.milliseconds = 50;
	pRec->flashDuration.seconds = 0;
	pRec->flashDuration.minutes = 0;
	pRec->pauseDuration.milliseconds = 50;
	pRec->pauseDuration.seconds = 0;
	pRec->pauseDuration.minutes = 0;
	pRec++;
	
	pRec->flashDuration.milliseconds = 70;
	pRec->flashDuration.seconds = 0;
	pRec->flashDuration.minutes = 0;
	pRec->pauseDuration.milliseconds = 70;
	pRec->pauseDuration.seconds = 0;
	pRec->pauseDuration.minutes = 0;
	pRec++;
	
	pRec->flashDuration.milliseconds = 150;
	pRec->flashDuration.seconds = 0;
	pRec->flashDuration.minutes = 0;
	pRec->pauseDuration.milliseconds = 0;
	pRec->pauseDuration.seconds = 1;
	pRec->pauseDuration.minutes = 0;
	pRec++;
	
	pBckgCmd->setDataBlockSize(sizeof(CommandStrobesDataHeader) + 
		3 * sizeof(CommandStrobesDataRecord));
		
	uint8_t secondSize = sizeof(IncomingCommand) + sizeof(CommandStrobesDataHeader) + 
		3 * sizeof(CommandStrobesDataRecord);
	
	// set total size of 'save' command
	pSaveCmd->setDataBlockSize(sizeof(EECommandData) + firstSize + secondSize);
	//pExec->executeCommand(pBckgCmd);
	pExec->executeCommand(pSaveCmd);
}

Color c;
int main(void)
{
	RGB_Led::init();
	uartInit();
	TimeIntervalGenerator::setupTimedPulse();
	
	CommExecutorFacade facade;
	
	facade.initialize();
	
	
	EEPlayer* pPlayer = facade.getEEPlayer();
	/*
	pPlayer->wipeOutPlayerData();
	Color::clear(&c);
	c.red = 255;
	initBlinkData(true);
	testSavingCommandToEEColor(&facade, &c, 0, 0);
	Color::clear(&c);
	c.green = 255;
	initBlinkData(false);
	testSavingCommandToEEColor(&facade, &c, 1, 0);
	Color::clear(&c);
	c.blue = 255;
	initBlinkData(true);
	testSavingCommandToEEColor(&facade, &c,  2, 0);
	Color::clear(&c);
	c.red = 255;
	c.blue = 255;
	initBlinkData(false);
	testSavingCommandToEEColor(&facade, &c, 3, 0);
	Color::clear(&c);
	c.red = 255;
	c.green = 255;
	c.blue = 255;
	initBlinkData(true);
	testSavingCommandToEEColor(&facade, &c, 4, 0);
	
	testSavingCommandToEESequence(&facade, 5, 0, true);
	*/
	//testSavingCommandExec(&facade);
	pPlayer->moveToCell(0);
	//pPlayer->moveToCell(5);
	
	/*
	pPlayer->back();
	pPlayer->forward();
	pPlayer->back();
	*/
	//pPlayer->forward();
	//pPlayer->moveToCell(1);
	//pPlayer->moveToCell(5);
	//pPlayer->forward();
	//testEECommand(&facade);
	//facade.getEEPlayer()->reloadCurrentCell();
	//pPlayer->wipeOutPlayerData();
	while (1) 
    {		
		facade.pollForCommand();
		facade.updateManually();
		//commandReceiver.receiveCommand();
    }
}

