/* 
* SourceOfData.h
*
* Created: 6/15/2016 10:18:38 PM
* Author: Alex
*/


#ifndef __SOURCEOFDATA_H__
#define __SOURCEOFDATA_H__


class SourceOfData
{
//functions
public:
	
	SourceOfData();
	virtual ~SourceOfData();
	virtual void initSource() = 0;
	virtual bool isDataAvailable() = 0;
	virtual char getNextByte() = 0;
	/* this method make sense only for pre-recorded sources (memory, eeprom). */
	virtual void resetDataSource() = 0;
	
	virtual void sendByte(char byte) = 0;

}; //SourceOfData

#endif //__SOURCEOFDATA_H__
