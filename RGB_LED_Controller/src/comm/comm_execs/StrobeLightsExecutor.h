/* 
* FlashingLightSequence.h
*
* Created: 7/31/2016 11:32:56 PM
* Author: Alex
*/


#ifndef __FLASHINGLIGHTSEQUENCE_H__
#define __FLASHINGLIGHTSEQUENCE_H__

#include "../src/comm/CommandExecutor.h"
#include "../src/timed_pulse/TimeInterval.h"
#include "../src/timed_sequence/SequencePlayer.h"
#include "../src/hardware_drivers/Strobe.h"

namespace StrobeRelated {

#define COMMAND_STROBE_SEQUENCE 2

typedef struct{
	// ignore flashes durations (turn off pulse checking if it is)
	uint8_t isItPermanent;
	// flag, which used only if isPermanent is set (!=0)
	uint8_t isON;
	// whether to repeat flashes all the time
	uint8_t repeat;
	// number of flashes in this sequence
	uint8_t numberOfFlashes;
} CommandStrobesDataHeader;

// data is organized in such a way that 'CommandFlashDataRecord' 
// can be represented as two 'TimeInterval' - first for light duration, and the 
// second - for pause. This behavior is implemented in state callback
typedef struct{
	TimeInterval flashDuration;
	TimeInterval pauseDuration;
} CommandStrobesDataRecord;

class StrobeTerminateCallback : public TimeIntervalGeneration::EventCallback
{
private:	
	Strobe* pStrobe;
	
public:
	virtual void onPulseStarted();
	virtual void onPulseEnded();
	void setStrobe(Strobe* pStrobe);
};

class StrobeCallback : public TimeIntervalGeneration::EventCallback
{
private:
	Strobe* pStrobe;
	TimeInterval* pData;
	
public:
	StrobeCallback();
	virtual void onPulseStarted();
	virtual void onPulseEnded();
	
	void setStrobe(Strobe* pStrobe);
	void setStrobeData(TimeInterval* pData);
	
};

class StrobeLightsExecutor : public CommandExecutor
{
//variables
public:
protected:
private:
	
	// strobe flashes is represented by two intervals - time during which
	// strobe active and inactivity time. We can play both intervals by
	// SequencePlayer.
	TimeInterval strobeDurations[2 * MAX_SEQUENCE_LENGTH];
	// reference to strobe driver
	Strobe* pStrobe;
	TimeIntervalGeneration::SequencePlayer* pSequencePlayer;
	
	StrobeCallback callback;
	StrobeTerminateCallback terminateCallback;
	
	
	// point to data header in command data block
	CommandStrobesDataHeader dataHeader;
	TimeInterval* pFirstInterval;
	
//functions
public:
	StrobeLightsExecutor();
	virtual ~StrobeLightsExecutor();
	
	virtual bool executeCommand(IncomingCommand* pCommand);
	
	virtual bool isRGBCommand();
	virtual bool isCommandResumable();
	
	virtual bool stopCommand(uint8_t commandCode);
	virtual bool resumeCommand(uint8_t commandCode);
	
	void setStrobe(Strobe* pStrobe);
	void setSequencePlayer(TimeIntervalGeneration::SequencePlayer* pPlayer);
	
protected:
private:
	StrobeLightsExecutor( const StrobeLightsExecutor &c );
	StrobeLightsExecutor& operator=( const StrobeLightsExecutor &c );
	
	void loadDataFromCommand(IncomingCommand* pCommand);
	void setupPlayerAndCallbacks();

}; //FlashingLightSequence

}
#endif //__FLASHINGLIGHTSEQUENCE_H__
