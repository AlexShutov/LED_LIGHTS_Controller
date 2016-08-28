/* 
* EEPlayerKeypadCallback.h
*
* Created: 8/27/2016 11:37:32 PM
* Author: Alex
*/


#ifndef __EEPLAYERKEYPADCALLBACK_H__
#define __EEPLAYERKEYPADCALLBACK_H__

#include "compiler.h"
#include "../src/comm/comm_execs/EE/EEPlayer.h"
#include "../src/hardware_drivers/keyboard/Keypad.h"

class PlayerKeypadCallback : public KeypadCallback
{
//variables
public:
protected:
private:
	EESupport::EEPlayer* pPlayer;
//functions
public:
	PlayerKeypadCallback();
	~PlayerKeypadCallback();
	
	void setPlayer(EESupport::EEPlayer* pEEPlayer);
	// Inherited from KeypadCallback
	virtual void onBackButton(bool isPress);
	virtual void onForwardButton(bool isPress);
	virtual void onStartButtonPressed();
	virtual void onStopButtonPressed();
	
protected:
private:
	PlayerKeypadCallback( const PlayerKeypadCallback &c );
	PlayerKeypadCallback& operator=( const PlayerKeypadCallback &c );

}; //EEPlayerKeypadCallback

#endif //__EEPLAYERKEYPADCALLBACK_H__
