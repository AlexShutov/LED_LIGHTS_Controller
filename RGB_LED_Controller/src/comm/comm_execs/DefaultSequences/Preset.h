/* 
* Preset.h
*
* Created: 8/28/2016 10:41:13 AM
* Author: Alex
*/


#ifndef __PRESET_H__
#define __PRESET_H__

#include "../src/comm/comm_execs/EE/EEPlayer.h"

namespace PresetData{

class Preset
{
//variables
public:
protected:
private:
	EESupport::EEPlayer* pPlayer;
//functions
public:
	
	bool restoreSequenceInCell(uint8_t cellIndex, char* commandBuffer);
	
	virtual char* writeRGBSequence(char* buffBeg, uint8_t* pSizeDst) = 0;
	virtual bool writeStrobeSequence(char* buffBeg) = 0;
	virtual bool hasStrobe() = 0;
	void setEEPlayer(EESupport::EEPlayer* pPlayer);
	
	Preset();
	~Preset();
protected:
private:
	Preset( const Preset &c );
	Preset& operator=( const Preset &c );

}; //Preset

}
#endif //__PRESET_H__
