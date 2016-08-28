/* 
* PolicePreset.cpp
*
* Created: 8/28/2016 12:13:53 PM
* Author: Alex
*/


#include "compiler.h"
#include "PolicePreset.h"

#include "../src/hardware_drivers/RGB_Led.h"
#include "../src/comm/comm_execs/ColorSequenceExecutor.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"

using namespace PresetData;
using namespace StrobeRelated;
using namespace LedCommandExecutors;

// default constructor
PolicePreset::PolicePreset()
{
} //PolicePreset

// default destructor
PolicePreset::~PolicePreset()
{
} //~PolicePreset

bool PolicePreset::hasStrobe()
{
	return true;
}

char* PolicePreset::writeRGBSequence(char* buffBeg, uint8_t* pSizeDst)
{
	// allocate command header at buffer start
	IncomingCommand* pCommand = (IncomingCommand*) buffBeg;
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
	
	// all colors have the same duration
	TimeInterval colorDuration;
	colorDuration.milliseconds = 100;
	colorDuration.seconds = 0;
	colorDuration.minutes = 0;
	
	// color #1
	//33 0 3 0xa 255 20 10
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 0;
	pRec->pulseColor.blue = 0;
	pRec->pulseDuration = colorDuration;
	// move to the next color
	pRec++;
	// color #2
	//33 0 3 0xa 245 245 22
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 0;
	pRec->pulseColor.green = 0;
	pRec->pulseColor.blue = 255;
	pRec->pulseDuration = colorDuration;
	// move to the next color
	
	
	uint8_t size = sizeof(CommColorHeader) +
	pHead->numberOfLights * sizeof(CommColorSequenceRecord);
	pCommand->setDataBlockSize(size);
	
	// save command with player (it will play that command after)
	//pPlayer->saveToCell(pCommand, cellNo, cellOffset);
	
	*pSizeDst = size + sizeof(IncomingCommand);
	return (char*)(pRec + 1);
}

bool PolicePreset::writeStrobeSequence(char* buffBeg)
{
	IncomingCommand* pCommand = (IncomingCommand*) buffBeg;
	pCommand->setCommandCode(COMMAND_STROBE_SEQUENCE);
	char* pDataBegin = (char*) (pCommand + 1);
	pCommand->setBufferPtr(pDataBegin);
	
	CommandStrobesDataHeader* pHead = (CommandStrobesDataHeader*) pDataBegin;
	pHead->isItPermanent = false;
	pHead->isON = false;
	pHead->numberOfFlashes = 3;
	pHead->repeat = 1;
	
	CommandStrobesDataRecord* pRec = (CommandStrobesDataRecord*)( pHead + 1);
	TimeInterval* pDuration = 0;
	TimeInterval* pPause = 0;
	// pulse #1
	pDuration = &pRec->flashDuration;
	pPause = &pRec->pauseDuration;
	// pulse ON time
	pDuration->milliseconds = 50;
	pDuration->seconds = 0;
	pDuration->minutes = 0;
	// pulse OFF time
	pPause->milliseconds = 50;
	pPause->seconds = 0;
	pPause->minutes = 0;
	pRec++;
	
	// pulse #2
	pDuration = &pRec->flashDuration;
	pPause = &pRec->pauseDuration;
	// pulse ON time
	pDuration->milliseconds = 70;
	pDuration->seconds = 0;
	pDuration->minutes = 0;
	// pulse OFF time
	pPause->milliseconds = 70;
	pPause->seconds = 0;
	pPause->minutes = 0;
	pRec++;
	
	// pulse #3
	pDuration = &pRec->flashDuration;
	pPause = &pRec->pauseDuration;
	// pulse ON time
	pDuration->milliseconds = 150;
	pDuration->seconds = 0;
	pDuration->minutes = 0;
	// pulse OFF time
	pPause->milliseconds = 0;
	pPause->seconds = 1;
	pPause->minutes = 0;
	pRec++;
	
	uint8_t dataSize = sizeof(CommandStrobesDataHeader) +
	pHead->numberOfFlashes * sizeof(CommandStrobesDataRecord);
	pCommand->setDataBlockSize(dataSize);
	return true;
}