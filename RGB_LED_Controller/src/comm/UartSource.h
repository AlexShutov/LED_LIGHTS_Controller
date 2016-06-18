/* 
* UartSource.h
*
* Created: 6/15/2016 10:26:31 PM
* Author: Alex
*/


#ifndef __UARTSOURCE_H__
#define __UARTSOURCE_H__

#include "SourceOfData.h"

class UartSource : public SourceOfData
{
//variables
public:
protected:
private:

//functions
public:
	UartSource();
	virtual ~UartSource();
	virtual void initSource();
	virtual bool isDataAvailable();
	virtual char getNextByte();
	virtual void resetDataSource();
	virtual void sendByte(char byte);
	
	
protected:
private:
	UartSource( const UartSource &c );
	UartSource& operator=( const UartSource &c );

}; //UartSource

#endif //__UARTSOURCE_H__
