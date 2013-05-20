/* This is the C main file.  This should also be compiled with a c compiler */
#include <stdio.h>         // This is included because printf is used.
#include "cheader.h"     // We are calling functions from the C source here. so we
                        //need the header for it.
 
int main()
{
    printf ("About to run the c function.\n");
    cSpeak(); // Call the C function to start everything.
    return 0;
}
