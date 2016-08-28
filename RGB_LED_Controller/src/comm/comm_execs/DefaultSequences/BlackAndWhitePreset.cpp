/* 
* BlackAndWhitePreset.cpp
*
* Created: 8/28/2016 12:30:03 PM
* Author: Alex
*/

#include "compiler.h"
#include "BlackAndWhitePreset.h"

#include "../src/hardware_drivers/RGB_Led.h"
#include "../src/comm/comm_execs/ColorSequenceExecutor.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"

using namespace PresetData;
using namespace StrobeRelated;
using namespace LedCommandExecutors;

// default constructor
BlackAndWhitePreset::BlackAndWhitePreset()
{
} //BlackAndWhitePreset

// default destructor
BlackAndWhitePreset::~BlackAndWhitePreset()
{
} //~BlackAndWhitePreset


bool BlackAndWhitePreset::hasStrobe()
{
	return true;
}


char* BlackAndWhitePreset::writeRGBSequence(char* buffBeg, uint8_t* pSizeDst)
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
	colorDuration.milliseconds = 300;
	colorDuration.seconds = 0;
	colorDuration.minutes = 0;
	
	// color #1
	//33 0 3 0xa 255 20 10
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 255;
	pRec->pulseColor.blue = 255;
	pRec->pulseDuration = colorDuration;
	// move to the next color
	pRec++;
	// color #2
	//33 0 3 0xa 245 245 22
	Color::clear(&pRec->pulseColor);
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

bool BlackAndWhitePreset::writeStrobeSequence(char* buffBeg)
{
	IncomingCommand* pCommand = (IncomingCommand*) buffBeg;
	pCommand->setCommandCode(COMMAND_STROBE_SEQUENCE);
	char* pDataBegin = (char*) (pCommand + 1);
	pCommand->setBufferPtr(pDataBegin);
	
	CommandStrobesDataHeader* pHead = (CommandStrobesDataHeader*) pDataBegin;
	pHead->isItPermanent = false;
	pHead->isON = false;
	pHead->numberOfFlashes = 1;
	pHead->repeat = 1;
	
	CommandStrobesDataRecord* pRec = (CommandStrobesDataRecord*)( pHead + 1);
	TimeInterval* pDuration = 0;
	TimeInterval* pPause = 0;
	// pulse #1
	pDuration = &pRec->flashDuration;
	pPause = &pRec->pauseDuration;
	// pulse ON time
	pDuration->milliseconds = 300;
	pDuration->seconds = 0;
	pDuration->minutes = 0;
	// pulse OFF time
	pPause->milliseconds = 300;
	pPause->seconds = 0;
	pPause->minutes = 0;
	pRec++;
	
	uint8_t dataSize = sizeof(CommandStrobesDataHeader) +
	pHead->numberOfFlashes * sizeof(CommandStrobesDataRecord);
	pCommand->setDataBlockSize(dataSize);
	return true;
}