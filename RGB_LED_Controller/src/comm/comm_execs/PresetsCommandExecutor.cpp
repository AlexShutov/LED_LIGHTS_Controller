/* 
* PresetsCommandExecutor.cpp
*
* Created: 8/28/2016 12:39:25 AM
* Author: Alex
*/


#include "PresetsCommandExecutor.h"
#include "../src/hardware_drivers/RGB_Led.h"

using namespace LedCommandExecutors;
using namespace EESupport;

// default constructor
PresetsCommandExecutor::PresetsCommandExecutor()
{
	pPlayer = 0;
} //PresetsCommandExecutor

// default destructor
PresetsCommandExecutor::~PresetsCommandExecutor()
{
} //~PresetsCommandExecutor

void PresetsCommandExecutor::setEEPlayer(EESupport::EEPlayer* pPlayer)
{
	this->pPlayer = pPlayer;
}

/************************************************************************/
/*				Inherited from CommandExecutor                          */
/************************************************************************/


bool PresetsCommandExecutor::isRGBCommand()
{
	return false;
}

bool PresetsCommandExecutor::isCommandResumable()
{
	return false;
}

bool PresetsCommandExecutor::stopCommand(uint8_t commandCode)
{
	return true;
}

bool PresetsCommandExecutor::resumeCommand(uint8_t commandCode)
{
	
	return true;
}

bool PresetsCommandExecutor::executeCommand(IncomingCommand* pCommand)
{
	if (getCommandCode() != pCommand->getCommandCode() || !pPlayer){
		return false;
	}
	
	if (pCommand->getDataBlockSize() != sizeof(PresetDataStructure)){
		// command has to have only PresetDataStructure, ot, perhaps,
		// data is corrupt - abort execution
		return false;
	}
	
	PresetDataStructure* pSettings = (PresetDataStructure*) pCommand->getBufferPtr();
	if (!pSettings->restoreDefaults && !pSettings->wipeOutEEPROM){
		// command tell us to do nothing, consider it done
		return true;
	}
	
	if (pSettings->wipeOutEEPROM && pPlayer){
		// don't call for now
		pPlayer->wipeOutPlayerData();
		// for now there is no saved data in eeprom, 
		// stop all current commands
		pPlayer->stopBackgroundCommands();
	}
	// if we need to restore default light sequences
	if (pSettings->restoreDefaults){
		// allocate buffer on stack big enough for to store single
		// block data from EEPROM
		char commandBuffer[BLOCK_SIZE];
		bool atLeastOneCellRestored = false;
		for (uint8_t i = 0; i < NUMBER_OF_MEMORY_CELL; ++i){
			// check if given cell is not in use
			if (pPlayer && !pPlayer->isCellInUse(i)){
				// and restore default if it is
				bool restored = restoreSequenceInCell(i, commandBuffer);
				// we use returned value because cell data may not be restored
				// because of some reason
				atLeastOneCellRestored |= restored;
			}
		}
		if (pSettings->wipeOutEEPROM && atLeastOneCellRestored){
			// we removed all data from EEPROM and restored
			// at least one cell (defined in code, because all cells is empty)
			// Now, load data from first cell
			if (pPlayer){
				pPlayer->reloadCurrentCell();
			}
		}
		// it may happen if all cells is occupied and command didn't
		// requested data purge.
		if (!atLeastOneCellRestored){
			return false;
		}
	}
	return true;
}

bool PresetsCommandExecutor::restoreSequenceInCell(uint8_t cellIndex, char* commandBuffer)
{
	char* secondCommBegin = 0;
	uint8_t rgbCommandSize = 0;
	// write first command into the buffer
	secondCommBegin = writeRGBSequence(commandBuffer, &rgbCommandSize);
	// write second (strobe) command into buffer
	bool hasSecondCommand = false;
	hasSecondCommand = writeStrobeSequence(secondCommBegin);
	
	if (cellIndex == 0){
		pPlayer->saveToCell(0, hasSecondCommand, 
			(IncomingCommand*) commandBuffer, (IncomingCommand*) secondCommBegin);
			return true;
	}
	return false;
}

char* PresetsCommandExecutor::writeRGBSequence(char* buffBeg, uint8_t* pSizeDst)
{
	IncomingCommand* pCommand = (IncomingCommand*) buffBeg;
	pCommand->setCommandCode(COMMAND_CODE_CHANGE_COLOR);
	pCommand->setDataBlockSize(sizeof(Color));
	//CommColorHeader* pHeader = (CommColorHeader*)( pCommand + 1);

	Color* pColor = (Color*)( pCommand + 1);
	pCommand->setBufferPtr((char*)pColor);
	pColor->red = 255;
	pColor->green = 20;
	pColor->blue = 10;
	RGB_Led::setColor(pColor);
	*pSizeDst = sizeof(IncomingCommand) + sizeof(Color);
	//return (char*) (pColor + 1);
	return buffBeg;
	/*
	Color* pColor = 0;
	TimeInterval* pDuration = 0;
	// setup sequence info
	pHeader->isSmoothSwitch = false;
	pHeader->repeat = true;
	pHeader->numberOfLights = 1;
	// calculate data block size
	uint8_t dataBlockSize = sizeof(CommColorHeader) +
	pHeader->numberOfLights * sizeof(CommColorSequenceRecord);
	pCommand->setDataBlockSize(dataBlockSize);
		
	CommColorSequenceRecord* pRec = (CommColorSequenceRecord*) (pHeader + 1);
	
	// light #1
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 255;
	pColor->green = 20;
	pColor->blue = 10;
	// setup duration
		pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	RGB_Led::setColor(pColor);
	// light #2
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 245;
	pColor->green = 245;
	pColor->blue = 22;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	/*
	// light #3
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 51;
	pColor->green = 255;
	pColor->blue = 255;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	
	// light #4
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 255;
	pColor->green = 102;
	pColor->blue = 0;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	
	// light #5
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 10;
	pColor->green = 255;
	pColor->blue = 50;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	
	// light #6
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 102;
	pColor->green = 0;
	pColor->blue = 255;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	
	// light #7
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 102;
	pColor->green = 255;
	pColor->blue = 51;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	
	// light #8
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 204;
	pColor->green = 153;
	pColor->blue = 0;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	
	// light #9
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 10;
	pColor->green = 255;
	pColor->blue = 20;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	
	// light #10
	pColor = &pRec->pulseColor;
	pDuration = &pRec->pulseDuration;
	// setup color
	pColor->red = 255;
	pColor->green = 20;
	pColor->blue = 10;
	// setup duration
	pDuration->milliseconds = 0;
	pDuration->seconds = 1;
	pDuration->minutes = 0;
	pRec++;
	
	
	
	*pSizeDst = sizeof(IncomingCommand) + dataBlockSize;
	char* nextCommandBeg = (char*) pRec;
	return nextCommandBeg;
	*/
}

bool PresetsCommandExecutor::writeStrobeSequence(char* buffBeg)
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












