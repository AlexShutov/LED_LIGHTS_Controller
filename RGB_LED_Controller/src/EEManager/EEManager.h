/* 
* EEManager.h
*
* Created: 8/16/2016 1:05:56 AM
* Author: Alex
*/


#ifndef __EEMANAGER_H__
#define __EEMANAGER_H__
#include <avr/eeprom.h>
#include "compiler.h"


namespace EESupport {

// statically defined parameters describing storage structure
#define BLOCK_SIZE 100
#define NUMBER_OF_BLOCKS 10
#define EE_STORAGE_SIZE BLOCK_SIZE * NUMBER_OF_BLOCKS

/* This class is supposed to be a singleton, but, unfortunately,
 it is a bad practice- to use 'new' on AVR controller - everything 
 is created on stack so please instantiate only one instance of 
 this class (in some 'facade' or in 'main' before infinite loop and 
 use this instance throughout program) */
class EEManager
{
//variables
public:
protected:
private:
	
	

//functions
public:
	EEManager();	
	~EEManager();
	
	/*
		Write data to constant memory, write starting point is defined
		by block index and offset within a block
	*/
	void writeData(uint8_t blockIndex, 
				   uint8_t blockOffset,
				   void* pSrc, 
				   uint8_t size);
	/* The same for reading data */
	void readData(uint8_t blockIndex,
				  uint8_t blockOffset,
				  void* pDst,
				  uint8_t size);
	/* Here is no 'clearBlock' method because EEPROM has limited 
	  number of write cycles. Data within block has to be used
	  responsively
	 */
	
	void writeBlock(uint8_t blockIndex, char* pSrc);
	void readBlock(uint8_t blockIndex, char* pDst);
	
protected:
private:
	EEManager( const EEManager &c );
	EEManager& operator=( const EEManager &c );
	/** 
	All operations with EEPROM has to be performed by methods from
	avr/eeprom.h library. I don't want that inside client code, so 
	all that stuff should be hidden from user
	*/
	char* getEEPtr(uint8_t blockIndex, uint8_t offset);
	


}; //EEManager

}
#endif //__EEMANAGER_H__
