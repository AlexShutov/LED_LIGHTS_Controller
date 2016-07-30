/* 
* LightSequenceExecutor.cpp
*
* Created: 7/29/2016 3:25:34 AM
* Author: Alex
*/


#include "ColorSequenceExecutor.h"

using namespace LedCommandExecutors;

// default constructor
ColorSequenceExecutor::ColorSequenceExecutor()
{
} //LightSequenceExecutor

// default destructor
ColorSequenceExecutor::~ColorSequenceExecutor()
{
} //~LightSequenceExecutor

void ColorSequenceExecutor::setSequencePlayer(TimeIntervalGeneration::SequencePlayer* pPlayer){
	pSequencPlayer = pPlayer;
}

TimeInterval* ColorSequenceExecutor::getTimeInterval(uint8_t index){
	
}

bool ColorSequenceExecutor::executeCommand(IncomingCommand* pCommand){
	
}

bool ColorSequenceExecutor::revertCommand(IncomingCommand* pCommand){
	
}
