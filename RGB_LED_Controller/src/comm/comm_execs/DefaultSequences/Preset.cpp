/* 
* Preset.cpp
*
* Created: 8/28/2016 10:41:12 AM
* Author: Alex
*/

#include "../src/comm/IncomingCommand.h"
#include "Preset.h"

using namespace PresetData;

// default constructor
Preset::Preset()
{
} //Preset

// default destructor
Preset::~Preset()
{
} //~Preset

void Preset::setEEPlayer(EESupport::EEPlayer* pPlayer)
{
	this->pPlayer = pPlayer;
}

bool Preset::restoreSequenceInCell(uint8_t cellIndex, char* commandBuffer)
{
	char* secondCommBegin = 0;
	uint8_t rgbCommandSize = 0;
	// write first command into the buffer
	secondCommBegin = writeRGBSequence(commandBuffer, &rgbCommandSize);
	// write second (strobe) command into buffer
	bool hasSecondCommand = hasStrobe();
	if (hasStrobe()){
		hasSecondCommand = writeStrobeSequence(secondCommBegin);
	}
	
	pPlayer->saveToCell(cellIndex, hasSecondCommand,
		(IncomingCommand*) commandBuffer, 
		hasStrobe() ? (IncomingCommand*) secondCommBegin : 0);
	return true;
	
	return false;
}