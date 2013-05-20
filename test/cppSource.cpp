/* This is the c++ source file.*/
#include "cppheader.h" // Include the cppheader.h file
#include <stdio.h> // We need to include this in order to use printf.
     
//This is the function we are trying to run from C (it needs a wrapper)
void myTest::sayHello() 
{
	printf ("This is how you say hello, using C that calls a C++ function\n");
}
     
//This function is meant to call the class function. This is the wrapper.
extern "C" void sayHelloWrapper() 
{
	printf("inside wrapper about to run sayHello\n");
	myTest* speak; // Create instance of class type 
	speak->sayHello(); // call sayHello()
     
}
     
/*  This file needs to be compiled by the c++ compiler.  ie: g++ */
