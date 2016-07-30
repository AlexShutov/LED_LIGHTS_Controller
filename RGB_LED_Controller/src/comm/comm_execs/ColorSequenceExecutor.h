/* 
* LightSequenceExecutor.h
*
* Created: 7/29/2016 3:25:34 AM
* Author: Alex
*/


#ifndef __LIGHTSEQUENCEEXECUTOR_H__
#define __LIGHTSEQUENCEEXECUTOR_H__

#include "../src/comm/CommandExecutor.h"
#include "../src/TimedSequence/SequencePlayer.h"
#include "../src/LED_RGB_Driver/RGB_Led.h"

namespace LedCommandExecutors {
	
#define COMMAND_CODE_LIGHT_SEQUENCE 1

/** Header of command, preceding data block */
typedef struct {
	char repeat;
	char numberOfLights;
	char isSmoothSwitch;
}CommColorHeader;

/** Data for single light */
typedef struct{
	Color pulseColor;
	TimeInterval pulseDuration;
}CommColorSequenceRecord;
#define COMMAND_BLINK_DATA_DATA_HEADER_SIZE sizeof(CommandBlinkDataHeader)


/************************************************************************/
/* Plays sequence of colors   
/************************************************************************/
class ColorSequenceExecutor  : public CommandExecutor
{
//variables
public:
protected:
	// Assign pointer to command's header and command's data block.
	// It is called when executor is being requested to handle command.
	void assignInternalData(IncomingCommand* pCommand);
private:
	/** Points to Sequence player, assigned to this kind of sequence */
	TimeIntervalGeneration::SequencePlayer* pSequencPlayer;
	// point to data header in received buffer
	CommColorHeader* pDataHeader;
	// points to first data record
	CommColorSequenceRecord* pDataRecords;
	// probably should move it to static field inside Color class
	Color colorOff;

//functions
public:
	ColorSequenceExecutor();
	virtual ~ColorSequenceExecutor();
		
	virtual bool executeCommand(IncomingCommand* pCommand);
	virtual bool revertCommand(IncomingCommand* pCommand);
	
	/** pull TimeInterval for i'th light from general command buffer */
	virtual TimeInterval* getTimeInterval(uint8_t index);
	/* Assign SequencePlayer to this command */
	void setSequencePlayer(TimeIntervalGeneration::SequencePlayer* pPlayer);
	
protected:
private:
	ColorSequenceExecutor( const ColorSequenceExecutor &c );
	ColorSequenceExecutor& operator=( const ColorSequenceExecutor &c );

}; //LightSequenceExecutor

}
#endif //__LIGHTSEQUENCEEXECUTOR_H__
