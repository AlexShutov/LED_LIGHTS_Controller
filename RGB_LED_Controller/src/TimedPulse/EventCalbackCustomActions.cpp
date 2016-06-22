/* 
* EventCalbackCustomActions.cpp
*
* Created: 6/21/2016 1:51:26 AM
* Author: Alex
*/


#include "EventCalbackCustomActions.h"

using namespace TimeIntervalGeneration;

// default constructor
EventCalbackCustomActions::EventCalbackCustomActions()
{
	for(uint8_t i = 0; i < MAX_CUSTOM_ACTIONS; ++i){
		customActions[i] = 0;
	}
} //EventCalbackCustomActions

// default destructor
EventCalbackCustomActions::~EventCalbackCustomActions()
{
} //~EventCalbackCustomActions

void EventCalbackCustomActions::setCustomAction(EventCallback* pAction, uint8_t actionIndex){
	if (actionIndex >= MAX_CUSTOM_ACTIONS) return;
	customActions[actionIndex] = pAction;
}

EventCallback* EventCalbackCustomActions::getCustomAction(uint8_t actionIndex){
	return (actionIndex >= MAX_CUSTOM_ACTIONS) ? 0 : customActions[actionIndex];
}

void EventCalbackCustomActions::setCustomActions(EventCallback* pActions, 
												 uint8_t actionsSize)
{
	for (uint8_t i = 0; i < actionsSize; ++i){
		setCustomAction(&pActions[i], i);
		// each action is responsible for one pulse (under its number)
	}	
}

/************************************************************************/
/*                      Inherited methods from callback                 */
/************************************************************************/
// dispatch call to custom action which position equals to item index
void EventCalbackCustomActions::onPulseStarted(){
	EventCallback* pAction = customActions[getPulseNo()];
	pAction->setPulseNo(getPulseNo());
	pAction->onPulseStarted();
}

void EventCalbackCustomActions::onPulseEnded(){
	EventCallback* pAction = customActions[getPulseNo()];
	pAction->setPulseNo(getPulseNo());
	pAction->onPulseEnded();
}
