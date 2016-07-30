/* 
* EventCalbackCustomActions.cpp
*
* Created: 6/21/2016 1:51:26 AM
* Author: Alex
*/


#include "EventCallbackCustomActions.h"

using namespace TimeIntervalGeneration;

// default constructor
EventCallbackCustomActions::EventCallbackCustomActions()
{
	for(uint8_t i = 0; i < MAX_CUSTOM_ACTIONS; ++i){
		customActions[i] = 0;
	}
} //EventCalbackCustomActions

// default destructor
EventCallbackCustomActions::~EventCallbackCustomActions()
{
} //~EventCalbackCustomActions

void EventCallbackCustomActions::setCustomAction(EventCallback* pAction, uint8_t actionIndex){
	if (actionIndex >= MAX_CUSTOM_ACTIONS) return;
	customActions[actionIndex] = pAction;
}
void EventCallbackCustomActions::setCustomActionToMany(EventCallback* pAction, 
	uint8_t actionsSize)
{
	for (uint8_t i = 0; i < actionsSize; ++i){
		setCustomAction(pAction, i);
		// each action is responsible for one pulse (under its number)
	}	
}

EventCallback* EventCallbackCustomActions::getCustomAction(uint8_t actionIndex){
	return (actionIndex >= MAX_CUSTOM_ACTIONS) ? 0 : customActions[actionIndex];
}

void EventCallbackCustomActions::setCustomActions(EventCallback* pActions, 
												 uint8_t actionsSize)
{
	for (uint8_t i = 0; i < actionsSize; ++i){
		customActions[i] = &pActions[i];
		// each action is responsible for one pulse (under its number)
	}	
}

/************************************************************************/
/*                      Inherited methods from callback                 */
/************************************************************************/
// dispatch call to custom action which position equals to item index
void EventCallbackCustomActions::onPulseStarted(){
	EventCallback* pAction = customActions[getItemIndex()];
	pAction->setItemIndex(getItemIndex());
	pAction->onPulseStarted();
}

void EventCallbackCustomActions::onPulseEnded(){
	EventCallback* pAction = customActions[getItemIndex()];
	pAction->setItemIndex(getItemIndex());
	pAction->onPulseEnded();
}
