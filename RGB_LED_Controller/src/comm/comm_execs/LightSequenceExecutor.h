/* 
* LightSequenceExecutor.h
*
* Created: 7/29/2016 3:25:34 AM
* Author: Alex
*/


#ifndef __LIGHTSEQUENCEEXECUTOR_H__
#define __LIGHTSEQUENCEEXECUTOR_H__

#include "../src/comm/CommandExecutor.h"

namespace LedCommandExecutors {
	
#define COMMAND_CODE_LIGHT_SEQUENCE 1

class LightSequenceExecutor  : public CommandExecutor
{
//variables
public:
protected:
private:

//functions
public:
	LightSequenceExecutor();
	virtual ~LightSequenceExecutor();
		
	virtual bool executeCommand(IncomingCommand* pCommand);
	virtual bool revertCommand(IncomingCommand* pCommand);
	
protected:
private:
	LightSequenceExecutor( const LightSequenceExecutor &c );
	LightSequenceExecutor& operator=( const LightSequenceExecutor &c );

}; //LightSequenceExecutor

}
#endif //__LIGHTSEQUENCEEXECUTOR_H__
