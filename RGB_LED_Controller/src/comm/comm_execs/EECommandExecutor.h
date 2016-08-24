/* 
* EECommandExecutor.h
*
* Created: 8/16/2016 2:44:18 AM
* Author: Alex
*/


#ifndef __EECOMMANDEXECUTOR_H__
#define __EECOMMANDEXECUTOR_H__

#include "../src/comm/CommandExecutor.h"
#include "../src/comm/comm_execs/EE/EEPlayer.h"

namespace EESupport {

#define COMMAND_EE 3

typedef struct {
	// ee storage's cell index executor
	// has to use
	uint8_t cellIndex;
	// indicates whether save or load 
	// from EEPROM
	bool isLoadCommand;
	// if command isn't inload mode, this flag will
	// tell executor to mark cell as not used
	bool eraseCell;
	// the next fields responsible for 
	// saving data
	// if data block has background command also
	bool hasBackgroundCommand;
	// we can calculate background command offset
	//uint8_t backgroundCommBlockBeginPos;
	
} EECommandData;

class EECommandExecutor : public CommandExecutor
{
//variables
public:
protected:
private:
	
	EEPlayer*  pEEPlayer;

//functions
public:
	EECommandExecutor();
	~EECommandExecutor();
	
	
	virtual bool isRGBCommand();
	virtual bool isCommandResumable();
	virtual bool stopCommand(uint8_t commandCode);
	virtual bool resumeCommand(uint8_t commandCode);
	
	virtual bool executeCommand(IncomingCommand* pCommand);
	
	
	void setEEPlayer(EESupport::EEPlayer* pPlayer);
	
protected:
private:
	EECommandExecutor( const EECommandExecutor &c );
	EECommandExecutor& operator=( const EECommandExecutor &c );
	
}; //EECommandExecutor

}
#endif //__EECOMMANDEXECUTOR_H__
