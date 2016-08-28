/* 
* BlackAndWhitePreset.h
*
* Created: 8/28/2016 12:30:04 PM
* Author: Alex
*/


#ifndef __BLACKANDWHITEPRESET_H__
#define __BLACKANDWHITEPRESET_H__

#include "Preset.h"

namespace PresetData {
	
class BlackAndWhitePreset : public Preset
{
//variables
public:
protected:
private:

//functions
public:
	BlackAndWhitePreset();
	~BlackAndWhitePreset();
	
	virtual char* writeRGBSequence(char* buffBeg, uint8_t* pSizeDst);
	virtual bool writeStrobeSequence(char* buffBeg);
	virtual bool hasStrobe();
	
protected:
private:
	BlackAndWhitePreset( const BlackAndWhitePreset &c );
	BlackAndWhitePreset& operator=( const BlackAndWhitePreset &c );

}; //BlackAndWhitePreset
}

#endif //__BLACKANDWHITEPRESET_H__
