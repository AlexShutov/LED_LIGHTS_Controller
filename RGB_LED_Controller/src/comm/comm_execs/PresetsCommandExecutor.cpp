/* 
* PresetsCommandExecutor.cpp
*
* Created: 8/28/2016 12:39:25 AM
* Author: Alex
*/


#include "PresetsCommandExecutor.h"
#include "../src/hardware_drivers/RGB_Led.h"

#include "../src/comm/comm_execs/DefaultSequences/Preset.h"
#include "../src/comm/comm_execs/DefaultSequences/CalmColorsPreset.h"
#include "../src/comm/comm_execs/DefaultSequences/SingleColorAndStrobePreset.h"
#include "../src/comm/comm_execs/DefaultSequences/PolicePreset.h"

using namespace LedCommandExecutors;
using namespace EESupport;
using namespace PresetData;
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
	
	switch (cellIndex){
		case 0: {
			// ten mild colors without strobe
			CalmColorsPreset preset;
			preset.setEEPlayer(pPlayer);
			preset.restoreSequenceInCell(cellIndex, commandBuffer);
			break;
		}
		case 1: {
			// police style pattern
			PolicePreset preset;
			preset.setEEPlayer(pPlayer);
			preset.restoreSequenceInCell(cellIndex, commandBuffer);
			break;
		}
		case 2: {
			// light yellow color
			SingleColorAndStrobePreset preset;
			preset.setEEPlayer(pPlayer);
			Color color;
			Color::clear(&color);
			color.red = 255;
			color.green = 255; 
			color.blue = 35;
			preset.setColor(&color);
			preset.restoreSequenceInCell(cellIndex, commandBuffer);
			break;
		}
		default:
			break;
	}
	
}


char* PresetsCommandExecutor::writeRGBSequence(char* buffBeg, uint8_t* pSizeDst)
{
	// allocate command header at buffer start
	IncomingCommand* pCommand = (IncomingCommand*) buffBeg;
	pCommand->setCommandCode(COMMAND_CODE_LIGHT_SEQUENCE);
	//	pCommand->setDataBlockSize(sizeof(Color));
	// write data block right after command header
	char* pDataPtr = (char*)(pCommand + 1);
	pCommand->setBufferPtr(pDataPtr);
	// form data
	// all colors have the sam duration
	
	// fill in command header
	CommColorHeader* pHead = (CommColorHeader*) pDataPtr;
	pHead->isSmoothSwitch = false;
	pHead->numberOfLights = 3;
	pHead->repeat = true;
	CommColorSequenceRecord* pRec = (CommColorSequenceRecord*) (pHead + 1);
	// color #1
	Color::clear(&pRec->pulseColor);
	// 
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 0;
	pRec->pulseColor.blue = 0;
	pRec->pulseDuration.milliseconds = 0;
	pRec->pulseDuration.seconds = 1;
	pRec->pulseDuration.minutes = 0;
	// move to the next color
	pRec++;
	// color #2
	Color::clear(&pRec->pulseColor);
	//
	pRec->pulseColor.red = 0;
	pRec->pulseColor.green = 255;
	pRec->pulseColor.blue = 0;
	pRec->pulseDuration.milliseconds = 0;
	pRec->pulseDuration.seconds = 1;
	pRec->pulseDuration.minutes = 0;
	// move to the next color
	pRec++;
	// color #3
	Color::clear(&pRec->pulseColor);
	//
	pRec->pulseColor.red = 255;
	pRec->pulseColor.green = 255;
	pRec->pulseColor.blue = 255;
	pRec->pulseDuration.milliseconds = 0;
	pRec->pulseDuration.seconds = 1;
	pRec->pulseDuration.minutes = 0;
	// move to the next color
	pRec++;
	uint8_t size = sizeof(CommColorHeader) + 2 * sizeof(CommColorSequenceRecord);
	pCommand->setDataBlockSize(size);
	
	// save command with player (it will play that command after)
	//pPlayer->saveToCell(pCommand, cellNo, cellOffset);
	
	*pSizeDst = size + sizeof(IncomingCommand);
	return (char*)(pRec + 1);
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












