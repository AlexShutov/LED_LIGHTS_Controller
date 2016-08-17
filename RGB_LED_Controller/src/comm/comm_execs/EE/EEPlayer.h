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

class EEPlayer
{
//variables
public:
protected:
private:
	EEManager* pEEManager;
	CommandExecutor* pCommandExec;
//functions
public:
	EEPlayer();
	~EEPlayer();
	
	
	void setEEManager(EEManager* pManager);
	EEManager* getEEManager();
	
	void setCommandExec(CommandExecutor* pExec);
	CommandExecutor* getCommandExec();
	
protected:
private:
	EEPlayer( const EEPlayer &c );
	EEPlayer& operator=( const EEPlayer &c );

}; //EEPlayer

}
#endif //__EEPLAYER_H__
