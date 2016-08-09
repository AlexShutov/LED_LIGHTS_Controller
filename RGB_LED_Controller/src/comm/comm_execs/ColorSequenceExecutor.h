/* 
* LightSequenceExecutor.h
*
* Created: 7/29/2016 3:25:34 AM
* Author: Alex
*/


#ifndef __LIGHTSEQUENCEEXECUTOR_H__
#define __LIGHTSEQUENCEEXECUTOR_H__

#include "../src/comm/CommandExecutor.h"
#include "../src/timed_sequence/SequencePlayer.h"
#include "../src/timed_pulse/EventCallbackCustomActions.h"
#include "../src/hardware_drivers/RGB_Led.h"

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


class SequenceTerminateCallback : public TimeIntervalGeneration::EventCallback {
private:
	Color c;
	public:
	virtual void onPulseStarted();
	virtual void onPulseEnded();
	virtual void setPulseNo(uint8_t pulseNo);
	
};

class ColorCallback : public TimeIntervalGeneration::EventCallback {
public:
	virtual void onPulseStarted();
	virtual void onPulseEnded();
	void setIsSmoothSwitching(bool isSmooth);
	void setColor(Color* pC, uint8_t size);
	
	
private:
	bool isSmoothSwitching;
	Color* pColor;
	uint8_t colorArSize;

};

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
	void loadData(IncomingCommand* pCommand);
	void setupPlayerAndCallbacks();
private:
	/** Points to Sequence player, assigned to this kind of sequence */
	TimeIntervalGeneration::SequencePlayer* pSequencPlayer;
	// point to data header in received buffer
	CommColorHeader dataHeader;
	// points to first data record
	CommColorSequenceRecord* pDataRecords;
	// probably should move it to static field inside Color class
	Color colorOff;
	
	Color intervalColors[MAX_SEQUENCE_LENGTH];
	TimeInterval intervalDurations[MAX_SEQUENCE_LENGTH];
	
	// this callback transform single callback into array of custom
	// callbacks, in which each item is responsible for its own interval
	TimeIntervalGeneration::EventCallbackCustomActions compositeCallback;
	
	// callback which is called when sequence have ended
	SequenceTerminateCallback terminateCallback;
	
	ColorCallback colorCallback;
	
//functions
public:
	ColorSequenceExecutor();
	virtual ~ColorSequenceExecutor();
		
	virtual bool executeCommand(IncomingCommand* pCommand);
	
	virtual bool isRGBCommand();
	virtual bool isCommandResumable();
	
	virtual bool stopCommand(uint8_t commandCode);
	virtual bool resumeCommand(uint8_t commandCode);
	
	/** pull TimeInterval for i light from general command buffer */
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
