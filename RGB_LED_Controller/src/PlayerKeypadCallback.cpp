/* 
* EEPlayerKeypadCallback.cpp
*
* Created: 8/27/2016 11:37:32 PM
* Author: Alex
*/


#include "PlayerKeypadCallback.h"

// default constructor
PlayerKeypadCallback::PlayerKeypadCallback()
{
} //EEPlayerKeypadCallback

// default destructor
PlayerKeypadCallback::~PlayerKeypadCallback()
{
} //~EEPlayerKeypadCallback

void PlayerKeypadCallback::setPlayer(EESupport::EEPlayer* pEEPlayer)
{
	pPlayer = pEEPlayer;
}

void PlayerKeypadCallback::onBackButton(bool isPress)
{
	if (isPress && pPlayer){
		pPlayer->back();
	}
}

void PlayerKeypadCallback::onForwardButton(bool isPress)
{
	if (isPress && pPlayer){
		pPlayer->forward();
	}
}

void PlayerKeypadCallback::onStartButtonPressed()
{
	if (pPlayer){
		pPlayer->reloadCurrentCell();
	}
}

void PlayerKeypadCallback::onStopButtonPressed()
{
	if (pPlayer){
		pPlayer->stopAllCommands();
	}
}