/* 
* CommandExecutor.cpp
*
* Created: 6/18/2016 10:24:49 AM
* Author: Alex
*/


#include "CommandExecutor.h"


/************************************************************************/
/*			Inherited from ExternalEndCallback                          */
/************************************************************************/

EndCallbackDispatcher::EndCallbackDispatcher()
{
	pExec = 0;
	pCallback = 0;
	pEndAction = 0;	
}

void EndCallbackDispatcher::onPulseStarted()
{
	if (!pExec) return;
	uint8_t commandCode = pExec->getCommandCode();
	if (pEndAction) pEndAction->onPulseStarted();
	if (pCallback) pCallback->onSequenceRestarted(commandCode);
}

void EndCallbackDispatcher::onPulseEnded()
{
	if (!pExec) return;
	uint8_t commandCode = pExec->getCommandCode();
	if (pEndAction) pEndAction->onPulseEnded();
	if (pCallback) pCallback->onSequenceEnded(commandCode);
}

void EndCallbackDispatcher::setExec(CommandExecutor* pExec)
{
	this->pExec = pExec;
}

void EndCallbackDispatcher::setExternalCallback(ExternalEndCallback* pCallback)
{
	this->pCallback = pCallback;
}

ExternalEndCallback* EndCallbackDispatcher::getExternalCallback()
{
	return pCallback;
}


void EndCallbackDispatcher::setEndAction(TimeIntervalGeneration::EventCallback* pAction)
{
	pEndAction = pAction;
}


/************************************************************************/
/*			Inherited from  CommandExecutor                             */
/************************************************************************/


// virtual destructor
CommandExecutor::~CommandExecutor()
{
	commandCode = 0;
} //~CommandExecutor

void CommandExecutor::setCommandCode(uint8_t code){
	commandCode = code;
}

uint8_t CommandExecutor::getCommandCode(){
	return commandCode;
}

void CommandExecutor::setExternalEndCallback(ExternalEndCallback* pCallback)
{	
	if (pEndCallbackDispatcher){
		pEndCallbackDispatcher->setExternalCallback(pCallback);
	}
}

ExternalEndCallback* CommandExecutor::getExternalEndCallback()
{
	return pEndCallbackDispatcher->getExternalCallback();
}

TimeIntervalGeneration::EventCallback* CommandExecutor::getDecoratedEndCallback()
{
	return pEndCallbackDispatcher;
}


void CommandExecutor::setEndCallbackDispatcher(EndCallbackDispatcher* pDispatcher)
{
	pEndCallbackDispatcher = pDispatcher;
}

void CommandExecutor::setEndAction(TimeIntervalGeneration::EventCallback* pAction)
{	
	if (pEndCallbackDispatcher){
		pEndCallbackDispatcher->setEndAction(pAction);
	}
}