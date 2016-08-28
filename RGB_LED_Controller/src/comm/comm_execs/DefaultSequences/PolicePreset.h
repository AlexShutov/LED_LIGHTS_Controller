/* 
* PolicePreset.h
*
* Created: 8/28/2016 12:13:53 PM
* Author: Alex
*/


#ifndef __POLICEPRESET_H__
#define __POLICEPRESET_H__

#include "Preset.h"

namespace PresetData {
	
class PolicePreset : public Preset
{
//variables
public:
protected:
private:

//functions
public:
	PolicePreset();
	~PolicePreset();
	
	virtual char* writeRGBSequence(char* buffBeg, uint8_t* pSizeDst);
	virtual bool writeStrobeSequence(char* buffBeg);
	virtual bool hasStrobe();
	
protected:
private:
	PolicePreset( const PolicePreset &c );
	PolicePreset& operator=( const PolicePreset &c );

}; //PolicePreset
}

#endif //__POLICEPRESET_H__
