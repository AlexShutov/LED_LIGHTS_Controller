/* 
* Log.cpp
*
* Created: 8/11/2016 11:50:21 PM
* Author: Alex
*/

#include "../src/include/uart_stuff.h"
#include "Log.h"

// default constructor
Log::Log()
{
} //Log

// default destructor
Log::~Log()
{
} //~Log

void Log::i(const char* message)
{
	uartPrint(message);
}