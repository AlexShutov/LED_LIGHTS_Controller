/* 
* EventCallbackDecorator.h
*
* Created: 6/19/2016 4:35:36 PM
* Author: Alex
*/


#ifndef __EVENTCALLBACKDECORATOR_H__
#define __EVENTCALLBACKDECORATOR_H__

#include "TimeIntervalGenerator.h"

namespace TimeIntervalGeneration {
	
class EventCallbackDecorator : public EventCallback
{
//variables
public:
protected:
private:
	EventCallback* pDecoree;
	EventCallback* pActionBefore;
	EventCallback* pActionAfter;

//functions
public:
	EventCallbackDecorator();
	~EventCallbackDecorator();
	
	void setDecoree(EventCallback* pAction);
	void setActionBefore(EventCallback* pBefore);
	void setActionAfter(EventCallback* pAfter);
	
	EventCallback* getDecoree();
	EventCallback* getActionBefore();
	EventCallback* getActionAfter();
	
	virtual void onPulseStarted();
	virtual void onPulseEnded();
	virtual void setItemIndex(uint8_t pulseNo);
	
protected:
private:
	EventCallbackDecorator( const EventCallbackDecorator &c );
	EventCallbackDecorator& operator=( const EventCallbackDecorator &c );

}; //EventCallbackDecorator

}
#endif //__EVENTCALLBACKDECORATOR_H__
