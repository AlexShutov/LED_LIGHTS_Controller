/* 
* SingleColorAndStrobePreset.h
*
* Created: 8/28/2016 11:48:11 AM
* Author: Alex
*/


#ifndef __SINGLECOLORANDSTROBEPRESET_H__
#define __SINGLECOLORANDSTROBEPRESET_H__

#include "Preset.h"
#include "../src/hardware_drivers/RGB_Led.h"
namespace PresetData{

class SingleColorAndStrobePreset : public Preset
{
//variables
public:
protected:
private:
	Color* pColor;
//functions
public:
	SingleColorAndStrobePreset();
	~SingleColorAndStrobePreset();
	
	void setColor(Color* pColor);
	
	virtual char* writeRGBSequence(char* buffBeg, uint8_t* pSizeDst);
	virtual bool writeStrobeSequence(char* buffBeg);
	virtual bool hasStrobe();
	
protected:
private:
	SingleColorAndStrobePreset( const SingleColorAndStrobePreset &c );
	SingleColorAndStrobePreset& operator=( const SingleColorAndStrobePreset &c );

}; //SingleColorAndStrobePreset

}
#endif //__SINGLECOLORANDSTROBEPRESET_H__
