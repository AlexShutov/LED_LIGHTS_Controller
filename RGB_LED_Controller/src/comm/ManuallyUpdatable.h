/* 
* ManuallyUpdatable.h
*
* Created: 8/14/2016 12:11:32 PM
* Author: Alex
*/


#ifndef __MANUALLYUPDATABLE_H__
#define __MANUALLYUPDATABLE_H__


class ManuallyUpdatable
{
//variables
public:
protected:
private:
	bool isManualUpdateModeActive;
	bool isManualUpdateNeeded;

//functions
public:
	ManuallyUpdatable();
	~ManuallyUpdatable();
	
	virtual void updateManually() = 0;
	// pass true to to activate manual update mode
	void setManualUpdate(bool isManual);	
	// figure out if this entity has to be updated manually 
	// (from main loop)
	bool isManualUpdate();
	

	
protected:
	// called from within derived class for marking entity as needed update
	void markAsNotUpdated();
	void markAsUpdated();
	bool isUpdateNeeded();
private:
	ManuallyUpdatable( const ManuallyUpdatable &c );
	ManuallyUpdatable& operator=( const ManuallyUpdatable &c );

}; //ManuallyUpdatable

#endif //__MANUALLYUPDATABLE_H__
