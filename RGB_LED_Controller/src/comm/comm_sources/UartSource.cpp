/* 
* UartSource.cpp
*
* Created: 6/15/2016 10:26:31 PM
* Author: Alex
*/


#include "UartSource.h"
extern "C" {
	#include "../src/include/uart_stuff.h"	
};

// default constructor
UartSource::UartSource()
{
} //UartSource

UartSource::~UartSource(){
	
}

void UartSource::initSource(){
	uartInit();
}

bool UartSource::isDataAvailable(){
	return uartAvail();
}

char UartSource::getNextByte(){
	return uartRecv();
}

void UartSource::resetDataSource(){
	
}

void UartSource::sendByte(char byte){
	return uartSend(byte);
}
