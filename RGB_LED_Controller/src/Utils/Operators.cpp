/*
 * Operators.cpp
 *
 * Created: 6/15/2016 10:58:54 PM
 *  Author: Alex
 */ 
#include <stdlib.h>
#include "operators.h"

 void* operator new(size_t size)
 {
	 return malloc(size);
 }

 void operator delete(void * ptr)
 {
	 free(ptr);
 }


 extern "C" void __cxa_pure_virtual();
 extern "C" void _pure_virtual(void);
 extern "C" void __pure_virtual(void);
 extern "C" int atexit( void (*func)(void));
 extern "C" int __cxa_atexit();
 void __cxa_pure_virtual() {
 }

 void _pure_virtual() {
 }

 void __pure_virtual() {
 }

 int atexit( void (*func)(void)) {return -1;}
 int __cxa_atexit() {return -1;}
