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
	colorOff.red = 0;
	colorOff.green = 0;
	colorOff.blue = 0;
} //LightSequenceExecutor

// default destructor
ColorSequenceExecutor::~ColorSequenceExecutor()
{
} //~LightSequenceExecutor

void ColorSequenceExecutor::setSequencePlayer(TimeIntervalGeneration::SequencePlayer* pPlayer){
	pSequencPlayer = pPlayer;
}

TimeInterval* ColorSequenceExecutor::getTimeInterval(uint8_t index){
	CommColorSequenceRecord* pRec = pDataRecords + index;
	TimeInterval* pDuration = &(pRec->pulseDuration);
	return pDuration;
}

/*
	Here is a tricky part: There is several RGB LED commands, each having its
	own structure of data record, but only one SequencePlayer. That mean, if we want to
	switch command executor, we should disable current executor first, and then ask another
	to execute command - it will set itself as mapper in SequencePlayer. Imaging, if we
	start executing another command while the old one is still running- SequencePlayer
	would have old data, but new data structure. 
*/
bool ColorSequenceExecutor::executeCommand(IncomingCommand* pCommand){
	// this is mandatory - see comment above
	pSequencPlayer->stopPlaying();
	// Use this executor as TimeInterval mapper
	assignInternalData(pCommand);
}
/* Stop playing current sequence and turn off RGB LED */
bool ColorSequenceExecutor::revertCommand(IncomingCommand* pCommand){
	pSequencPlayer->stopPlaying();
	RGB_Led::setColor(&colorOff);
}

void ColorSequenceExecutor::assignInternalData(IncomingCommand* pCommand){
	char* pBuffer = pCommand->getBufferPtr();
	pDataHeader = (CommColorHeader*) pBuffer;
	// section with color data comes right after command data header
	pDataRecords = (CommColorSequenceRecord*) ( pDataHeader + 1);
}
