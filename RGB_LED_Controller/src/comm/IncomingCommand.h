/* 
* IncomingCommand.h
*
* Created: 6/17/2016 11:52:35 PM
* Author: Alex
*/


#ifndef __INCOMINGCOMMAND_H__
#define __INCOMINGCOMMAND_H__

#include <avr/io.h>

class IncomingCommand
{
//variables
public:
protected:
private:
	// how many bytes in command's data block
	uint8_t dataBlockSize;
	// point to command buffer (supposed to be the same, but who knows)
	char* pDataBuffer;
	// we need command code to process this command
	uint8_t commandCode;
	
//functions
public:
	IncomingCommand();
	~IncomingCommand();
	
	// accessors
	void setDataBlockSize(uint8_t size);
	void setBufferPtr(char* pBuffer);
	void setCommandCode(uint8_t code);
	
	uint8_t getDataBlockSize();
	char* getBufferPtr();
	uint8_t getCommandCode();
	
protected:
private:
	IncomingCommand( const IncomingCommand &c );
	IncomingCommand& operator=( const IncomingCommand &c );

}; //IncomingCommand

#endif //__INCOMINGCOMMAND_H__
