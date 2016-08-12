/* 
* Log.h
*
* Created: 8/11/2016 11:50:21 PM
* Author: Alex
*/


#ifndef __LOG_H__
#define __LOG_H__

class Log
{
//variables
public:
protected:
private:

//functions
public:
	
	static void i(const char* message);

	Log();
	~Log();
protected:
private:
	Log( const Log &c );
	Log& operator=( const Log &c );

}; //Log

#endif //__LOG_H__
