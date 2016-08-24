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
#include "../src/comm/CommandReceiver.h"
#include "../src/comm/IncomingCommand.h"
#include "../src/hardware_drivers/RGB_Led.h"

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
	// r/w buffer for EEPROM operation - read into it first, then
	// execute command
	char buffer[BLOCK_SIZE];
	// copy command info into temporary object, because interrupt callback 
	// may somehow corrupt it and loading from EEPROM need some command 
	// info object
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

	
	// wipes out all PlayerData records in memory instance
	// call 'savePlayerDataToEEPROM()' explicitly
	void wipeOutPlayerData();
	
	// return number of cells containing saved data
	uint8_t getNumberOfCellsInUse();
	bool isCellInUse(uint8_t cellIndex);
	
	// get indexes of first and last used cells 
	uint8_t getFirstUsedCellIndex();
	uint8_t getLastUsedCellIndex();
	
	// checks if player has next occupied cell
	bool hasNextUsedCell(uint8_t currentCellIndex);
	uint8_t getNextUsedCellIndex(uint8_t currCellIndex);
	// the same for 'back' direction
	bool hasPreviousUsedCell(uint8_t currentCellIndex);
	uint8_t getPreviousUsedCellIndex(uint8_t currentCellIndex);
	
	/* validate value and fix if it is corrupted. It may not be
	 initialized yet, in this case if there is at least one used cell, we select
	 first cell as current.
	 Another case is if current cell is set but this cell i sbeing marked as empty.
	 Current value will become corrupted. To fix that select previous cell if there 
	 is any used cell before, or next cell otherwise
	 */
	void validateCurrentCellValue();
	
	// tell player to take next used cell and process data from it
	void forward();
	// move to previous cell
	void back();
	// move to cell with explicit index - cell if cell is used, it
	// get selected as current and its data gets processed
	void moveToCell(uint8_t cellIndex);
	
	// process cell after it being selected
	void loadAndProcessCell(uint8_t cellIndex);
	
	
	void saveToCell(uint8_t cellIndex, 
					bool isHavingBackgroundCommand, 
					IncomingCommand* pRGBCommandHeader,
					IncomingCommand* pBackgroundCommandHeader);
	
	
	
	PlayerData* getPlayerData();
	
protected:
private:
	EEPlayer( const EEPlayer &c );
	EEPlayer& operator=( const EEPlayer &c );

	void loadPlayerDataFromEEPROM();
	void savePlayerDataToEEPROM();

	// check if validation symbols doesn't match. checking is done 
	// only during device startup
	bool checkIfPlayerDataCorrupted();
	
	// clear PlayerData in memory (without overwriting eeprom)
	void wipOutPlayerDataLocal();
	
	// helper method, saves command data into EEPROM cell.
	void saveToCellInner(IncomingCommand* pCommand,
	uint8_t cellIndex,
	uint8_t cellOffset);
	// load command from given cell starting from offset 'cellOffset'.
	// We validate method arguments first - reload player state data and
	// check if this sell is in use. If it is - proceed further.
	// Command is supposed to be having the same format as during saving -
	// first comes command header (IncomingCommand) and after it comes
	// data block.
	// All data is copied into internal buffer, after which data block is
	// set in command header. If CommandExecutor is not null, it executes
	// that command.
	// This method differ from 'loadAndProcessSell' because it has
	// cell offset. We can reuse it for loading two commands-
	// rgb and background one
	bool loadFromCellInner(uint8_t cellIndex, uint8_t cellOffset);
	

}; //EEPlayer

}
#endif //__EEPLAYER_H__
