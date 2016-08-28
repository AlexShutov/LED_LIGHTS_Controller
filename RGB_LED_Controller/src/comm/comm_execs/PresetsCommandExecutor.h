/* 
* PresetsCommandExecutor.h
*
* Created: 8/28/2016 12:39:25 AM
* Author: Alex
*/


#ifndef __PRESETSCOMMANDEXECUTOR_H__
#define __PRESETSCOMMANDEXECUTOR_H__

#include "../src/comm/CommandExecutor.h"
#include "../src/comm/comm_execs/EE/EEPlayer.h"


#include "../src/comm/comm_execs/ExecChangeColor.h"
#include "../src/comm/comm_execs/ColorSequenceExecutor.h"
#include "../src/comm/comm_execs/StrobeLightsExecutor.h"

using namespace StrobeRelated;

namespace LedCommandExecutors {

#define COMMAND_RESTORE_PRESETS 4

/*
	There is a three options for restoring default values in EEPROM:
	1) purge all data: 
		wipeOutEEPROM = true, restorePresets = false
	2) remove all data and restore presets:
		wipeOutEEPROM = true, restorePresets = true
	3) don't remove user data, restore default values in unoccupied cells:
		wipeOutEEPROM = false, restorePresets = true
*/
typedef struct {
	bool wipeOutEEPROM;
	bool restoreDefaults;	
} PresetDataStructure;

class PresetsCommandExecutor : public CommandExecutor
{
//variables
public:
protected:
private:
	// In order to save presets we have to use EEProm player
	// (EEPlayer)
	EESupport::EEPlayer* pPlayer;
	
	ExecChangeColor* pExecChangeColor;
	ColorSequenceExecutor* pColorSequenceExec;
	StrobeLightsExecutor* pStrobeExec;
	
//functions
public:
	PresetsCommandExecutor();
	~PresetsCommandExecutor();
	
	void setExecChangeColor(ExecChangeColor* pExec){
		pExecChangeColor = pExec;
	}
	void setColorSequenceExec(ColorSequenceExecutor* pExec){
		pColorSequenceExec = pExec;
	}
	void setStrobeExec(StrobeLightsExecutor* pExec){
		pStrobeExec = pExec;
	}
	
	void setEEPlayer(EESupport::EEPlayer* pPlayer);
	// even though this command alter data about RGB, and forces EEPlayer
	// to load first sequence eventually, it is not considered being RGB command,
	// nor resumable command
	virtual bool isRGBCommand();
	virtual bool isCommandResumable();
	virtual bool resumeCommand(uint8_t commandCode);
	virtual bool stopCommand(uint8_t commandCode);
	virtual bool executeCommand(IncomingCommand* pCommand);
	
protected:
private:
	PresetsCommandExecutor( const PresetsCommandExecutor &c );
	PresetsCommandExecutor& operator=( const PresetsCommandExecutor &c );
	
	bool restoreSequenceInCell(uint8_t cellIndex, char* commandBuffer);
	
	// Write sequence data into the buffer. buffBeg - begin of buffer,
	// pSizeDst- how many bytes command data took, return value - byte 
	// after command's block end - beginning of another command
	char* writeRGBSequence(char* buffBeg, uint8_t* pSizeDst);
	// compared to 'writeRGBSequence', this method does not return 
	// any pointer
	bool writeStrobeSequence(char* buffBeg);


}; //PresetsCommandExecutor

}
#endif //__PRESETSCOMMANDEXECUTOR_H__
