/* 
* EEManager.cpp
*
* Created: 8/16/2016 1:05:56 AM
* Author: Alex
*/


#include "EEManager.h"

using namespace EESupport;

char EEMEM eeStorage[EE_STORAGE_SIZE];

// default constructor
EEManager::EEManager()
{
} //EEManager

// default destructor
EEManager::~EEManager()
{
} //~EEManager

char* EEManager::getEEPtr(uint8_t blockIndex, uint8_t offset)
{
	return eeStorage + blockIndex * BLOCK_SIZE + offset;
}

void EEManager::writeData(uint8_t blockIndex, 
						  uint8_t blockOffset, 
						  void* pSrc, 
						  uint8_t size )
{
	char* pDst = getEEPtr(blockIndex, blockOffset);
	eeprom_update_block(pSrc, pDst, size);							  
}

void EEManager::readData(uint8_t blockIndex, 
						 uint8_t blockOffset, 
						 void* pDst, 
						 uint8_t size)
{
	char* pSrc = getEEPtr(blockIndex, blockOffset);
	eeprom_read_block(pDst, (const void*) pSrc, size);
	
}

void EEManager::writeBlock(uint8_t blockIndex, char* pSrc)
{
	writeData(blockIndex, 0, pSrc, BLOCK_SIZE);
}

void EEManager::readBlock(uint8_t blockIndex, char* pDst)
{
	readData(blockIndex, 0, (void*) pDst, BLOCK_SIZE);
}
