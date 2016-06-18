/* 
* CommandReceiver.cpp
*
* Created: 6/17/2016 10:49:21 PM
* Author: Alex
*/


#include "CommandReceiver.h"
#include "../src/LED_RGB_Driver/RGB_Led.h"
#include "../src/include/uart_stuff.h"

// default constructor
CommandReceiver::CommandReceiver()
{
	g_index = 0;
	db_index = 0;
	// dummy values
	commandToSend.setBufferPtr((char*) data_buffer);
	commandToSend.setCommandCode(-1);
	commandToSend.setDataBlockSize(0);
} //CommandReceiver

// default destructor
CommandReceiver::~CommandReceiver()
{
} //~CommandReceiver

void CommandReceiver::setSourceOfData(SourceOfData* pSource){
	if (0 != pSource){
		this->pSourceOfData = pSource;
	}
}

void CommandReceiver::setCommandExecutor(CommandExecutor* pExec){
	pExecutor = pExec;
}

void CommandReceiver::receiveCommand(){
	
  pSourceOfData->resetDataSource();
  while(pSourceOfData->isDataAvailable()) {
    g_buffer[g_index] = pSourceOfData->getNextByte();
    if(!((g_index==0)&&(g_buffer[0]!= TRAILING_SYMBOL))) {
      g_index++;
      if(g_buffer[g_index-1]== '\n') {
        if(g_index == INFO_HEADER_SIZE) {
		  read_data_chunk();
		  CommandHeader* pH = (CommandHeader*) g_buffer;
		  // executor should verify if command is correct, not receiver
		  /*
		  if (pH->commandCode >= 4){
			  acknowledgeCommand(false);
			  return;
		  }
		  */
		  // command is received, handle it
		  uint8_t commandCode = pH->commandCode;
		  commandToSend.setCommandCode(pH->commandCode);
		  commandToSend.setDataBlockSize(pH->dataSize);
		  if (pExecutor){
			  bool success = true;
			  success = pExecutor->executeCommand(&commandToSend);
			  // send result of processing command
			  acknowledgeCommand(success);
		  }
		g_index = 0;
	  }
      if(g_index >= INFO_HEADER_SIZE) {
        skipTheRestOfCommand();
        }
      }
    }
  }
}

void CommandReceiver::read_data_chunk()
{
	uint8_t data_chunk_size = ((CommandHeader*) g_buffer)->dataSize;
	for (uint8_t i = 0; i < data_chunk_size; ++i){
		while (!pSourceOfData->isDataAvailable());
		data_buffer[i] = pSourceOfData->getNextByte();
	}
}


void CommandReceiver::skipTheRestOfCommand()
{
	// Move forward to the next start character (if one is present)
	uint8_t index, start;
	for(start=1;(start<g_index)&&(g_buffer[start]!='!');start++);
	for(index=0;start<g_index;index++,start++)
	g_buffer[index] = g_buffer[start];
	g_index = index;
	// Reject command
	acknowledgeCommand(false);
}

void CommandReceiver::acknowledgeCommand(bool is_success){
	if (is_success)
		sendStringToSource(PSTR("+\n"));
	else
		sendStringToSource(PSTR("-\n"));
}

void CommandReceiver::sendStringToSource(const char *cszString) {
	uint8_t ch;
	while((ch = pgm_read_byte_near(cszString))!='\0') {
		pSourceOfData->sendByte(ch);
		cszString++;
	}
}