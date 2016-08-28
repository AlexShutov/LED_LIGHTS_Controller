/* 
* CalmColorsPreset.cpp
*
* Created: 8/28/2016 10:47:30 AM
* Author: Alex
*/

#include "compiler.h"
#include "CalmColorsPreset.h"
#include "../src/hardware_drivers/RGB_Led.h"
#include "../src/comm/comm_execs/ColorSequenceExecutor.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"

using namespace PresetData;
using namespace StrobeRelated;
using namespace LedCommandExecutors;
// default constructor
CalmColorsPreset::CalmColorsPreset()
{
} //CalmColorsPreset

// default destructor
CalmColorsPreset::~CalmColorsPreset()
{
} //~CalmColorsPreset

bool CalmColorsPreset::hasStrobe()
{
	return false;
}

char* CalmColorsPreset::writeRGBSequence(char* buffBeg, uint8_t* pSizeDst)
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
	pHead->numberOfLights = 10;
	pHead->repeat = true;
	CommColorSequenceRecord* pRec = (CommColorSequenceRecord*) (pHead + 1);
	
	// all colors have the same duration
	TimeInterval colorDuration;
	colorDuration.milliseconds = 0;
	colorDuration.seconds = 1;
	colorDuration.minutes = 0;
	
	// color #1
	//33 0 3 0xa 255 20 10
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 20;
	pRec->pulseColor.blue = 10;
	pRec->pulseDuration = colorDuration;
	// move to the next color
	pRec++;
	// color #2
	//33 0 3 0xa 245 245 22
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 245;
	pRec->pulseColor.green = 245;
	pRec->pulseColor.blue = 22;
	pRec->pulseDuration = colorDuration;
	// move to the next color
	pRec++;
	// color #3
	//33 0 3 0xa 51 255 255
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 51;
	pRec->pulseColor.green = 255;
	pRec->pulseColor.blue = 255;
	pRec->pulseDuration = colorDuration;
	pRec++;
	// move to the next color
	// color #4
	//33 0 3 0xa 255 102 0
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 102;
	pRec->pulseColor.blue = 0;
	pRec->pulseDuration = colorDuration;
	pRec++;
	// move to the next color
	// color #5
	//33 0 3 0xa 10 255 50
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 10;
	pRec->pulseColor.green = 255;
	pRec->pulseColor.blue = 50;
	pRec->pulseDuration = colorDuration;
	pRec++;
	// move to the next color
	// color #6
	//33 0 3 0xa 102 0 255
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 102;
	pRec->pulseColor.green = 0;
	pRec->pulseColor.blue = 255;
	pRec->pulseDuration = colorDuration;
	pRec++;
	// move to the next color
	// color #7
	//33 0 3 0xa 102 255 51
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 102;
	pRec->pulseColor.green = 255;
	pRec->pulseColor.blue = 51;
	pRec->pulseDuration = colorDuration;
	pRec++;
	// move to the next color
	// color #8
	//33 0 3 0xa 204 153 0
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 204;
	pRec->pulseColor.green = 153;
	pRec->pulseColor.blue = 0;
	pRec->pulseDuration = colorDuration;
	pRec++;
	// move to the next color
	// color #9
	//33 0 3 0xa 10 255 20
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 10;
	pRec->pulseColor.green = 255;
	pRec->pulseColor.blue = 20;
	pRec->pulseDuration = colorDuration;
	pRec++;
	// move to the next color
	// color #10
	//33 0 3 0xa 255 20 10
	Color::clear(&pRec->pulseColor);
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 255;
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

bool CalmColorsPreset::writeStrobeSequence(char* buffBeg)
{
	
	
}