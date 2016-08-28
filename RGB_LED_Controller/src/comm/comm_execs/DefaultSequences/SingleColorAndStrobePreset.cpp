/* 
* SingleColorAndStrobePreset.cpp
*
* Created: 8/28/2016 11:48:11 AM
* Author: Alex
*/

#include "compiler.h"
#include "SingleColorAndStrobePreset.h"

#include "../src/hardware_drivers/RGB_Led.h"
#include "../src/comm/comm_execs/ColorSequenceExecutor.h"
#include "../src/comm/comm_execs/ExecChangeColor.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"

using namespace PresetData;
using namespace StrobeRelated;
using namespace LedCommandExecutors;

// default constructor
SingleColorAndStrobePreset::SingleColorAndStrobePreset()
{
	pColor = 0;
} //SingleColorAndStrobePreset

// default destructor
SingleColorAndStrobePreset::~SingleColorAndStrobePreset()
{
} //~SingleColorAndStrobePreset

bool SingleColorAndStrobePreset::hasStrobe()
{
	return true;
}

void SingleColorAndStrobePreset::setColor(Color* pColor){
	this->pColor = pColor;
}

char* SingleColorAndStrobePreset::writeRGBSequence(char* buffBeg, uint8_t* pSizeDst)
{
	    // allocate command header at buffer start
    IncomingCommand* pCommand = (IncomingCommand*) buffBeg;
    pCommand->setCommandCode(COMMAND_CODE_CHANGE_COLOR);
    pCommand->setDataBlockSize(sizeof(Color));
    // write data block right after command header
    char* pDataPtr = (char*)(pCommand + 1);
    pCommand->setBufferPtr(pDataPtr);
    // set color
    Color* pBuffColor = (Color*) pDataPtr;
    if (!pColor){
		// use 'OFF' color if color not set
		pBuffColor->red = 0;
		pBuffColor->green = 0;
		pBuffColor->blue = 0;
	}else {
		// copy color into buffer
		*pBuffColor = *pColor;
	}
	*pSizeDst = sizeof(IncomingCommand) + sizeof(Color);
	return (char*) (pBuffColor + 1);
}

bool SingleColorAndStrobePreset::writeStrobeSequence(char* buffBeg)
{
	IncomingCommand* pCommand = (IncomingCommand*) buffBeg;
	pCommand->setCommandCode(COMMAND_STROBE_SEQUENCE);
	char* pDataBegin = (char*) (pCommand + 1);
	pCommand->setBufferPtr(pDataBegin);
	
	CommandStrobesDataHeader* pHead = (CommandStrobesDataHeader*) pDataBegin;
	pHead->isItPermanent = false;
	pHead->isON = false;
	pHead->numberOfFlashes = 4;
	pHead->repeat = 1;
	
	CommandStrobesDataRecord* pRec = (CommandStrobesDataRecord*)( pHead + 1);
	TimeInterval* pDuration = 0;
	TimeInterval* pPause = 0;
	// pulse #1
	pDuration = &pRec->flashDuration;
	pPause = &pRec->pauseDuration;
	// pulse ON time
	pDuration->milliseconds = 10;
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
	pDuration->milliseconds = 20;
	pDuration->seconds = 0;
	pDuration->minutes = 0;
	// pulse OFF time
	pPause->milliseconds = 60;
	pPause->seconds = 0;
	pPause->minutes = 0;
	pRec++;
	
	// pulse #3
	pDuration = &pRec->flashDuration;
	pPause = &pRec->pauseDuration;
	// pulse ON time
	pDuration->milliseconds = 40;
	pDuration->seconds = 0;
	pDuration->minutes = 0;
	// pulse OFF time
	pPause->milliseconds = 100;
	pPause->seconds = 0;
	pPause->minutes = 0;
	pRec++;
	
	// pulse #4
	pDuration = &pRec->flashDuration;
	pPause = &pRec->pauseDuration;
	// pulse ON time
	pDuration->milliseconds = 70;
	pDuration->seconds = 0;
	pDuration->minutes = 0;
	// pulse OFF time
	pPause->milliseconds = 500;
	pPause->seconds = 1;
	pPause->minutes = 0;
	pRec++;
	
	uint8_t dataSize = sizeof(CommandStrobesDataHeader) +
	pHead->numberOfFlashes * sizeof(CommandStrobesDataRecord);
	pCommand->setDataBlockSize(dataSize);
	return true;
}