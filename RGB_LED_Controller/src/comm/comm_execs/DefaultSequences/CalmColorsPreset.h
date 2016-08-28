/* 
* CalmColorsPreset.h
*
* Created: 8/28/2016 10:47:30 AM
* Author: Alex
*/


#ifndef __CALMCOLORSPRESET_H__
#define __CALMCOLORSPRESET_H__

#include "Preset.h"

namespace PresetData {

class CalmColorsPreset : public Preset
{
//variables
public:
protected:
private:

//functions
public:
	CalmColorsPreset();
	~CalmColorsPreset();
	
	virtual char* writeRGBSequence(char* buffBeg, uint8_t* pSizeDst);
	virtual bool writeStrobeSequence(char* buffBeg);
	virtual bool hasStrobe();
	
protected:
private:
	CalmColorsPreset( const CalmColorsPreset &c );
	CalmColorsPreset& operator=( const CalmColorsPreset &c );
	
	

}; //CalmColorsPreset

}
#endif //__CALMCOLORSPRESET_H__
