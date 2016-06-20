/* 
* EventCallbackDecorator.cpp
*
* Created: 6/19/2016 4:35:36 PM
* Author: Alex
*/


#include "EventCallbackDecorator.h"

using namespace TimeIntervalGeneration;

// default constructor
EventCallbackDecorator::EventCallbackDecorator()
{
	pDecoree = 0;
	pActionBefore = 0;
	pActionAfter = 0;
} //EventCallbackDecorator

// default destructor
EventCallbackDecorator::~EventCallbackDecorator()
{
} //~EventCallbackDecorator

void EventCallbackDecorator::setDecoree(EventCallback* pAction){
	pDecoree = pAction;
}
void EventCallbackDecorator::setActionBefore(EventCallback* pBefore){
	pActionBefore = pBefore;
}
void EventCallbackDecorator::setActionAfter(EventCallback* pAfter){
	pActionAfter = pAfter;
}

EventCallback* EventCallbackDecorator::getDecoree(){
	return pDecoree;
}
EventCallback* EventCallbackDecorator::getActionBefore(){
	return pActionBefore;
}
EventCallback* EventCallbackDecorator::getActionAfter(){
	return pActionAfter;
}

void EventCallbackDecorator::onPulseStarted(){
	if (pActionBefore) pActionBefore->onPulseStarted();
	if (pDecoree) pDecoree->onPulseStarted();
	if (pActionAfter) pActionAfter->onPulseStarted();
}

void EventCallbackDecorator::onPulseEnded(){
	if (pActionBefore) pActionBefore->onPulseEnded();
	if (pDecoree) pDecoree->onPulseEnded();
	if (pActionAfter) pActionAfter->onPulseEnded();
}

void EventCallbackDecorator::setPulseNo(uint8_t pulseNo){
	if (pActionBefore) pActionBefore->setPulseNo(pulseNo);
	if (pDecoree) pDecoree->setPulseNo(pulseNo);
	if (pActionAfter) pActionAfter->setPulseNo(pulseNo);
}