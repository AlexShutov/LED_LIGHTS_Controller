/* 
* IncomingCommand.cpp
*
* Created: 6/17/2016 11:52:34 PM
* Author: Alex
*/


#include "IncomingCommand.h"

// default constructor
IncomingCommand::IncomingCommand()
{
	dataBlockSize = 0;
	pDataBuffer = 0;
} //IncomingCommand

// default destructor
IncomingCommand::~IncomingCommand()
{
} //~IncomingCommand

void IncomingCommand::setDataBlockSize(uint8_t size){
	dataBlockSize = size;
}

void IncomingCommand::setBufferPtr(char* pBuffer){
	pDataBuffer = &(*pBuffer);
}

void IncomingCommand::setCommandCode(uint8_t code){
	commandCode = code;
}

uint8_t IncomingCommand::getDataBlockSize(){
	return dataBlockSize;
}

char* IncomingCommand::getBufferPtr(){
	return pDataBuffer;
}

 uint8_t IncomingCommand::getCommandCode(){
	 return commandCode;
 }



