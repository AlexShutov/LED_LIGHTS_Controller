/* 
* EventCalbackCustomActions.h
*
* Created: 6/21/2016 1:51:26 AM
* Author: Alex
*/


#ifndef __EVENTCALBACKCUSTOMACTIONS_H__
#define __EVENTCALBACKCUSTOMACTIONS_H__

#include "avr/io.h"
#include "TimeIntervalGenerator.h"
#include "EventCalbackCustomActions.h"

namespace TimeIntervalGeneration {

#define MAX_CUSTOM_ACTIONS 20

class EventCalbackCustomActions : public EventCallback
{
//variables
public:
protected:
private:
	
	EventCallback* customActions[MAX_CUSTOM_ACTIONS];
	
//functions
public:

	void setCustomAction(EventCallback* pAction, uint8_t actionIndex);
	EventCallback* getCustomAction(uint8_t actionIndex);

	void setCustomActions(EventCallback* pActions, uint8_t actionsSize);

	virtual void onPulseStarted();
	virtual void onPulseEnded();
	
	
	EventCalbackCustomActions();
	~EventCalbackCustomActions();
protected:
private:
	EventCalbackCustomActions( const EventCalbackCustomActions &c );
	EventCalbackCustomActions& operator=( const EventCalbackCustomActions &c );

}; //EventCalbackCustomActions

}
#endif //__EVENTCALBACKCUSTOMACTIONS_H__
