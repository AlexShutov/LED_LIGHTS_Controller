/* 
* CommandReceiver.h
*
* Created: 6/17/2016 10:49:21 PM
* Author: Alex
*/


#ifndef __COMMANDRECEIVER_H__
#define __COMMANDRECEIVER_H__

#include <avr/io.h>
#include "SourceOfData.h"
#include "IncomingCommand.h"
#include "CommandExecutor.h"

#define DATA_BUFFER_SIZE 200
#define INFO_HEADER_SIZE sizeof(CommandHeader) 
#define TRAILING_SYMBOL '!'

// format of data header
struct CommandHeader{
	uint8_t trailingSymbol;
	uint8_t commandCode;
	uint8_t dataSize;
	uint8_t newLineSymbol;
};



class CommandReceiver
{
//variables
public:
protected:
private:

	SourceOfData* pSourceOfData;	
	//! Input buffer
	uint8_t g_buffer[INFO_HEADER_SIZE];
	uint8_t data_buffer[DATA_BUFFER_SIZE];
	//! Input index
	uint8_t g_index;
	uint8_t db_index;
	
	// will execute parsed command
	CommandExecutor* pExecutor;
	// here we don't have concurrency, so use single
	// command for each CommandReceiver instance
	IncomingCommand commandToSend;

//functions
public:
	CommandReceiver();
	~CommandReceiver();
	
	void setSourceOfData(SourceOfData* pSource);
	void setCommandExecutor(CommandExecutor* pExec);
	void receiveCommand();
	
protected:
private:
	CommandReceiver( const CommandReceiver &c );
	CommandReceiver& operator=( const CommandReceiver &c );
	
	void read_data_chunk();
	void skipTheRestOfCommand();
	
	void acknowledgeCommand(bool is_success);
	
	void sendStringToSource(const char *cszString); 

}; //CommandReceiver

#endif //__COMMANDRECEIVER_H__
