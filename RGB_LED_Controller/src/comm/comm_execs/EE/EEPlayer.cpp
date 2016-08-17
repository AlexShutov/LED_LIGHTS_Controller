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
