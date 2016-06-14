/* 
* A.h
*
* Created: 6/13/2016 9:51:55 PM
* Author: Alex
*/


#ifndef __A_H__
#define __A_H__

#include <avr/io.h>

/************************************************************************/
/* the same, as LED_COLOR - pointer cast is used (keep the same field 
/* order
/************************************************************************/
class Color{
	public:
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};
	
class RGB_Led
{
//variables
public:
protected:
private:

//functions
public:
	
	static void init();
	static void setColor(Color* pColor);
	static void shiftToColor(Color* pColor);
	static void nextRandomColor(void);
	static Color* getNextColor(void);

	RGB_Led();
	~RGB_Led();
protected:
private:
	RGB_Led( const RGB_Led &c );
	RGB_Led& operator=( const RGB_Led &c );

}; //A

#endif //__A_H__
