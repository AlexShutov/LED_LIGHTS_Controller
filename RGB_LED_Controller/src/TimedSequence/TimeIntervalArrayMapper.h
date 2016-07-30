/* 
* ArrayMapper.h
*
* Created: 7/29/2016 11:33:02 PM
* Author: Alex
*/


#ifndef __ARRAYMAPPER_H__
#define __ARRAYMAPPER_H__

#include "../src/TimedPulse/TimeInterval.h"
/************************************************************************/
/* Sequence player usually takes array of TimeInterval. But, in some 
/* cases, like 'LightSequence' command data, we may have TimeInterval
/* instances mixed up with other data and we need to get address of
/* TimeInterval under given index.
/************************************************************************/
class TimeIntervalArrayMapper
{
//functions
public:
	TimeIntervalArrayMapper();
	virtual ~TimeIntervalArrayMapper();
	
	virtual TimeInterval* getTimeInterval(uint8_t index) = 0;
	
}; //ArrayMapper

#endif //__ARRAYMAPPER_H__
