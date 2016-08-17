/* 
* EEPlayer.cpp
*
* Created: 8/17/2016 1:55:37 AM
* Author: Alex
*/


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
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_CELL; ++i){
		CellData* pCurrCell = &playerData.savedPatternsInfo[i];
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
	playerData.currentCellIndex = 0;
	for (uint8_t i = 0; i < NUMBER_OF_MEMORY_CELL; ++i){
		CellData* pCurrCell = &playerData.savedPatternsInfo[i];
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
