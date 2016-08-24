/* 
* EEPlayer.cpp
*
* Created: 8/17/2016 1:55:37 AM
* Author: Alex
*/

#include <stdio.h>
#include <string.h>
#include "EEPlayer.h"


using namespace EESupport;

// default constructor
EEPlayer::EEPlayer()
{
} //EEPlayer

// default destructor
EEPlayer::~EEPlayer()
{
} //~EEPlayer

void EEPlayer::setEEManager(EEManager* pManager)
{
	pEEManager = pManager;
}

EEManager* EEPlayer::getEEManager()
{
	return pEEManager;
}

void EEPlayer::setCommandExec(CommandExecutor* pExec)
{
	pCommandExec = pExec;
}

CommandExecutor* EEPlayer::getCommandExec()
{
	return pCommandExec;
}

PlayerData* EEPlayer::getPlayerData()
{
	return &playerData;
}

void EEPlayer::init()
{
	loadPlayerDataFromEEPROM();
	if (checkIfPlayerDataCorrupted()){
		// remove all data locally and from EEPROM as well
		wipeOutPlayerData();
	}
	// check is current cell index in loaded data is valid
	validateCurrentCellValue();
}

void EEPlayer::loadPlayerDataFromEEPROM()
{
	if (!pEEManager) {
		return;
	}
	pEEManager->readData(PLAYER_STATE_DATA_BLOCK, PLAYER_DATA_BLOCK_OFFSET, 
			&playerData, sizeof(PlayerData));
}

void EEPlayer::savePlayerDataToEEPROM()
{
	if (!pEEManager){
		return;
	}
	pEEManager->writeData(PLAYER_STATE_DATA_BLOCK, PLAYER_DATA_BLOCK_OFFSET,
		&playerData, sizeof(PlayerData));
}

bool EEPlayer::checkIfPlayerDataCorrupted()
{
	if (playerData.corruptionDetectionSymbol != CORRUPTION_DETECTION_SYMBOL){
		return true;
	}
	CellData* pCurrCell = 0;
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_CELL; ++i){
		pCurrCell = &playerData.savedPatternsInfo[i];
		if (pCurrCell->corruptionDetectionSymbol != CORRUPTION_DETECTION_SYMBOL){
			// data is corrupted, don't risk and wipe out all saved data
			// (we could erase just this block otherwise)
			return true;
		}
	}
	return false;
}

void EEPlayer::wipOutPlayerDataLocal()
{
	playerData.corruptionDetectionSymbol = CORRUPTION_DETECTION_SYMBOL;
	// set invalid value - next after last cell index
	playerData.currentCellIndex = NUMBER_OF_MEMORY_CELL;
	CellData* pCurrCell = 0;
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_CELL; ++i){
		pCurrCell = &playerData.savedPatternsInfo[i];
		pCurrCell->corruptionDetectionSymbol = CORRUPTION_DETECTION_SYMBOL;
		pCurrCell->isInUse = 0;
		pCurrCell->isHavingBackgroundCommand = 0;
		pCurrCell->backgroundBlockBegin = 0;
	}
}

void EEPlayer::wipeOutPlayerData()
{
	loadPlayerDataFromEEPROM();
	wipOutPlayerDataLocal();
	savePlayerDataToEEPROM();
}

uint8_t EEPlayer::getNumberOfCellsInUse()
{
	if (checkIfPlayerDataCorrupted()){
		return 0;
	}
	uint8_t activeCnt = 0;
	CellData* pCurrCell = 0;
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_CELL; ++i){
		pCurrCell = &playerData.savedPatternsInfo[i];
		if (pCurrCell->isInUse){
			activeCnt++;
		}
	}
	return activeCnt;
}

bool EEPlayer::isCellInUse(uint8_t cellIndex)
{
	if (cellIndex >= NUMBER_OF_MEMORY_CELL){
		// index is out of range;
		return false;
	}
	return playerData.savedPatternsInfo[cellIndex].isInUse;
}

uint8_t EEPlayer::getFirstUsedCellIndex()
{
	CellData* pCurrCell = 0;
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_CELL; i++){
		pCurrCell = &playerData.savedPatternsInfo[i];
		if (pCurrCell->isInUse){
			return i;
		}
	}
}

uint8_t EEPlayer::getLastUsedCellIndex()
{
	CellData* pCurrCell = 0;
	for (uint8_t i = NUMBER_OF_MEMORY_CELL - 1; i >= 0; i--){
		pCurrCell = &playerData.savedPatternsInfo[i];
		if (pCurrCell->isInUse){
			return i;
		}
	}
}

bool EEPlayer::hasNextUsedCell(uint8_t currentCellIndex)
{
	if (currentCellIndex == NUMBER_OF_MEMORY_CELL) {
		return false;
	}
	// move to next cell
	CellData* pCurrCell = 0;
	++currentCellIndex;
	for (uint8_t i = currentCellIndex; i < NUMBER_OF_MEMORY_CELL; i++){
		pCurrCell = &playerData.savedPatternsInfo[i];
		if (pCurrCell->isInUse){
			return true;
		}
	}
	return false;
}

uint8_t EEPlayer::getNextUsedCellIndex(uint8_t currCellIndex)
{
	++currCellIndex;
	CellData* pCurrCell = 0;
	for (uint8_t i = currCellIndex; i < NUMBER_OF_MEMORY_CELL; i++){
		pCurrCell = &playerData.savedPatternsInfo[i];
		if (pCurrCell->isInUse){
			return i;
		}
	}
}

bool EEPlayer::hasPreviousUsedCell(uint8_t currentCellIndex){
	if (currentCellIndex == 0) {
		return false;
	}
	CellData* pCurrCell = 0;
	int8_t i = currentCellIndex -1;
	do {
		pCurrCell = &playerData.savedPatternsInfo[i];
		if (pCurrCell->isInUse){
			return true;
		}
	} while(--i >= 0);
	return false;
}

uint8_t EEPlayer::getPreviousUsedCellIndex(uint8_t currentCellIndex)
{
	CellData* pCurrCell = 0;
	int8_t i = currentCellIndex -1;
	do {
		pCurrCell = &playerData.savedPatternsInfo[i];
		if (pCurrCell->isInUse){
			return i;
		}
	} while(--i >= 0);
	return 0;
}

void EEPlayer::validateCurrentCellValue()
{
	// no active cells at all
	if (getNumberOfCellsInUse() == 0){
		playerData.currentCellIndex = NUMBER_OF_MEMORY_CELL;
		return;
	}
	if (isCellInUse(playerData.currentCellIndex)){
		// cell in use, everythin g is ok, aborting
		return;
	}
	// in case player wasn't initialized or data were wiped out
	if (playerData.currentCellIndex == NUMBER_OF_MEMORY_CELL){
		// there is active cell but current index wasn't updated
		if (getNumberOfCellsInUse() != 0){
			// select first cell
			playerData.currentCellIndex = getFirstUsedCellIndex();
		}
	}else {
		// case when active cell was marked as empty
		if (!isCellInUse(playerData.currentCellIndex)){
			if (getNumberOfCellsInUse() == 0){
				// there is no active cells at all, set index 
				// after last allowed
				playerData.currentCellIndex = NUMBER_OF_MEMORY_CELL;
			} else {
				if (hasPreviousUsedCell(playerData.currentCellIndex)){
					// there is some used cell before current
					playerData.currentCellIndex = 
						getPreviousUsedCellIndex(playerData.currentCellIndex);
				} else {
					// there is at least one active cell, it isn't current one and
					// there is no active cells before current, so it is next cell
					playerData.currentCellIndex = 
						getNextUsedCellIndex(playerData.currentCellIndex);
				}
			}
		}
	}
}

void showError(){
	Color c;
	c.red = 255;
	c.green = 0;
	c.blue= 255;
	RGB_Led::setColor(&c);
}

void EEPlayer::forward()
{
	bool wasInvalide = false;
	if (!isCellInUse(playerData.currentCellIndex)){
		wasInvalide = true;
		validateCurrentCellValue();
	}
	uint8_t numberOfUsedCells = getNumberOfCellsInUse();
	if (numberOfUsedCells == 0){
		return;
	}
	if (wasInvalide){
		// error branch, but now  
		// we have changed valid value, use it
		loadAndProcessCell(playerData.currentCellIndex);
		return;
	}
	// there is only one cell and it is current, do nothing
	if (numberOfUsedCells == 1){
		return;
	}
	// try to select next value
	if (!hasNextUsedCell(playerData.currentCellIndex) ){
		// we currently have last cell selected as current one, 
		// try to find cell from begin of a sequence
		playerData.currentCellIndex = 0;
		if (!isCellInUse(0)){
			// first cell is not in use, take the next one
			playerData.currentCellIndex = getNextUsedCellIndex(0);
		}
	} else {
		playerData.currentCellIndex = 
			getNextUsedCellIndex(playerData.currentCellIndex);
	}
	// now we have next cell index in 'current' field,
	// process that cell
	loadAndProcessCell(playerData.currentCellIndex);
}

void EEPlayer::back()
{
	bool wasInvalide = false;
	if (!isCellInUse(playerData.currentCellIndex)){
		wasInvalide = true;
		validateCurrentCellValue();
	}
	uint8_t numberOfUsedCells = getNumberOfCellsInUse();
	
	if (numberOfUsedCells == 0){
		return;
	}
	if (wasInvalide){
		// error branch, but now
		// we have changed valid value, use it
		loadAndProcessCell(playerData.currentCellIndex);
		return;
	}
	// there is only one cell and it is current, do nothing
	if (numberOfUsedCells == 1){
		return;
	}
	// try to select next value
	if (!hasPreviousUsedCell(playerData.currentCellIndex) ){
		// we currently have last cell selected as current one,
		// try to find cell from begin of a sequence
		playerData.currentCellIndex = NUMBER_OF_MEMORY_CELL - 1;
		if (!isCellInUse(NUMBER_OF_MEMORY_CELL - 1)){
			// first cell is not in use, take the next one
			playerData.currentCellIndex =
				getPreviousUsedCellIndex(NUMBER_OF_MEMORY_CELL - 1);
		}
		} else {
		playerData.currentCellIndex =
			getPreviousUsedCellIndex(playerData.currentCellIndex);
	}
	// now we have next cell index in 'current' field,
	// process that cell
	loadAndProcessCell(playerData.currentCellIndex);
}

void EEPlayer::moveToCell(uint8_t cellIndex)
{
	if (!isCellInUse(cellIndex)){
		// do nothing
		return;
	}
	// set new cell as current
	playerData.currentCellIndex = cellIndex;
	loadAndProcessCell(cellIndex);
}


void EEPlayer::loadAndProcessCell(uint8_t cellIndex)
{
	if (!pCommandExec){
		return;
	}
	loadFromCell(cellIndex, 0);

}

void EEPlayer::saveToCell(IncomingCommand* pCommand, 
						  uint8_t cellIndex,
						  uint8_t cellOffset)
{
	if (!pCommand) return;
	
	// copy command data into internal buffer
	IncomingCommand* pSavedCommHeader = (IncomingCommand*) buffer;
	*pSavedCommHeader = *pCommand;
	void* pDataBuffer = (pSavedCommHeader + 1);
	// copy command data from command into internal buffer
	memcpy(pDataBuffer, pSavedCommHeader->getBufferPtr(), 
		   pSavedCommHeader->getDataBlockSize());
	// now we have entire command data in our buffer (with command header first)
	// save it to EEPROM
	// we save command to EEPROM with offset 'cellOffset'. It is
	// necessary if when you want to save two commands to one cell
	
	pEEManager->writeData(cellIndex, cellOffset, buffer, 
		sizeof(IncomingCommand) + pSavedCommHeader->getDataBlockSize());
	// mark this cell as active
	playerData.savedPatternsInfo[cellIndex].isInUse = true;
	// save player header (it updates values, not rewrites -
	// safe for EEPROM)
	savePlayerDataToEEPROM();
}

bool EEPlayer::loadFromCell(uint8_t cellIndex, uint8_t cellOffset)
{
	// refresh player state and
	// validate data
	if (cellIndex >= NUMBER_OF_MEMORY_CELL || 
		!playerData.savedPatternsInfo[cellIndex].isInUse){
			return false;
	}
	// check if executor is set, no point in continuing 
	// without it
	if (!pCommandExec){
		return false;
	}
	// We need to know data block size, so copy 
	// load command header from EEPROM first
	pEEManager->readData(cellIndex, cellOffset, buffer, sizeof(IncomingCommand));
	IncomingCommand* pCommand = (IncomingCommand*) buffer;
	char* pDataBlock = (char*) (pCommand + 1);
	// copy the rest of data from EEPROM, starting from byte next to
	// command header
	pEEManager->readData(cellIndex, cellOffset + sizeof(IncomingCommand), 
		pDataBlock, pCommand->getDataBlockSize());
	// All command data is now in memory buffer; set pointer to 
	// data block in command header so executor can read command data
	pCommand->setBufferPtr(pDataBlock);
	// handle command by executor - it know what to do
	pCommandExec->executeCommand(pCommand);
	return true;
}