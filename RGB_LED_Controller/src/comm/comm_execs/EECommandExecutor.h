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

#define COMMAND_EE 3

class EECommandExecutor : public CommandExecutor
{
//variables
public:
protected:
private:
	
	EESupport::EEPlayer*  pEEPlayer;

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

#endif //__EECOMMANDEXECUTOR_H__
