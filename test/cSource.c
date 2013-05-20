/* This file is small for readibility. */
#include "cheader.h" //Include the cheader.h file.  It contains the cSpeak() protoype
#include <stdio.h> // We have to include this to use printf.
     
// C function designed to call a C++ function.
void cSpeak()
{
	printf("Inside cspeak.. running wrapper.\n");
	sayHelloWrapper();
}
     
/* This file needs to be compiled by the C compiler. ie gcc -c */
