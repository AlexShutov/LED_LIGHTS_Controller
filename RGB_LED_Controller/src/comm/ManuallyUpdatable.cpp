/* 
* ManuallyUpdatable.cpp
*
* Created: 8/14/2016 12:11:32 PM
* Author: Alex
*/


#include "ManuallyUpdatable.h"

// default constructor
ManuallyUpdatable::ManuallyUpdatable()
{
	isManualUpdateModeActive = false;
	isManualUpdateNeeded = false;
} //ManuallyUpdatable

// default destructor
ManuallyUpdatable::~ManuallyUpdatable()
{
} //~ManuallyUpdatable

void ManuallyUpdatable::setManualUpdate(bool isManual)
{
	// assume entity initially is updated
	isManualUpdateNeeded = false;
	isManualUpdateModeActive = isManual;
}

bool ManuallyUpdatable::isManualUpdate()
{
	return isManualUpdateModeActive;
}

void ManuallyUpdatable::markAsNotUpdated()
{
	isManualUpdateNeeded = true;
}

void ManuallyUpdatable::markAsUpdated()
{
	isManualUpdateNeeded = false;
}

bool ManuallyUpdatable::isUpdateNeeded()
{
	return isManualUpdateNeeded;
}