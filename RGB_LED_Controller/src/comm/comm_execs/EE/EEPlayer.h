/* 
* EEPlayer.h
*
* Created: 8/17/2016 1:55:37 AM
* Author: Alex
*/


#ifndef __EEPLAYER_H__
#define __EEPLAYER_H__
#include "../src/EEManager/EEManager.h"
#include "../src/comm/CommandExecutor.h"

namespace EESupport {
// Avr's Atmega328P has 1kb of EEPROM - 10 blocks by 100bytes.
//
#define PLAYER_STATE_DATA_BLOCK 7
// cells indexes range [0..6]
#define NUMBER_OF_MEMORY_CELL 7 

#define CORRUPTION_DETECTION_SYMBOL 0xAA

// Offset within data block for storing player data in EEPROM
#define PLAYER_DATA_BLOCK_OFFSET 0

/* Information for describing one cell of data. Player will have 
   one for each cell it use and will persist it in EEPROM when
   command is saved to EEPROM
 */
typedef struct {
	// EEPROM may be rewritten accidentally, so we need a way to
	// detect that. Solution is in writing one extra byte with very
	// rear pattern (10101010)
	uint8_t corruptionDetectionSymbol;
	// indicates if cell is in use
	uint8_t isInUse;
	// cell length is 100 bytes - it is enough for storing
	// one long command (for rgb diode only) or two short commands
	// (one for rgb and one background command (for strobe))
	uint8_t isHavingBackgroundCommand;
	// in case when we store two command in one cell (rgb and background)
	// rgb command may be much longed than background command so it is
	// wise to not to use 50 bytes for both commands but place background 
	// command right after rgb command. Unfortunately, it will complicate
	// saving algorithm and command for saving data will have one more field.
	uint8_t backgroundBlockBegin;
} CellData;

typedef struct {
	// see CellData variable comment
	uint8_t corruptionDetectionSymbol;
	// keeps info about memory cells, storing command data in EEPROM
	CellData savedPatternsInfo[NUMBER_OF_MEMORY_CELL];
	// Player keep track of 
	uint8_t currentCellIndex;
	
} PlayerData;


class EEPlayer
{
//variables
public:
protected:
private:
	EEManager* pEEManager;
	CommandExecutor* pCommandExec;
	
	PlayerData playerData;
//functions
public:
	EEPlayer();
	~EEPlayer();
	// checks if data saved in EEPROM is not corrupted and loads all data
	void init();
	
	void setEEManager(EEManager* pManager);
	EEManager* getEEManager();
	
	void setCommandExec(CommandExecutor* pExec);
	CommandExecutor* getCommandExec();
	
	void loadPlayerDataFromEEPROM();
	void savePlayerDataToEEPROM();
	

	
	PlayerData* getPlayerData();
	
protected:
private:
	EEPlayer( const EEPlayer &c );
	EEPlayer& operator=( const EEPlayer &c );

	// check if validation symbols doesn't match
	bool checkIfPlayerDataCorrupted();
	
	// clear PlayerData in memory (without overwriting eeprom)
	void wipOutPlayerDataLocal();
	// wipes out all PlayerData records in memory instance
	// call 'savePlayerDataToEEPROM()' explicitly
	void wipeOutPlayerData();

}; //EEPlayer

}
#endif //__EEPLAYER_H__
