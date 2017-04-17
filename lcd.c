//------------------------------------------------------------------------------------------------------------
// ODROID-C 16x2 LCD output application.
// Compile: gcc -o lcd.out lcd.c -lwiringPi -lwiringPiDev -lpthread
// Run : ./lcd.out "First string" "Second string"
// Requires user access to gpio. Max 16 characters per string.
//------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <unistd.h>
#include <string.h>
#include <time.h>
 
#include <wiringPi.h>
#include <lcd.h>
 
static int lcdHandle  = 0;
 
int main (int argc, char *argv[])
{
    if ( argc != 3 ) {
        printf( "usage: %s string1 string2", argv[0] );
    } else {
        wiringPiSetup();
        lcdHandle = lcdInit(2,16,4,7,0,2,3,1,4,0,0,0,0);

        if (lcdHandle < 0) {
            fprintf (stderr, "%s: lcdInit failed\n", argv [0]) ;
            return -1 ;
        }
        
        lcdPosition (lcdHandle, 0, 0);
        lcdPuts (lcdHandle, argv[1]);
        
        lcdPosition (lcdHandle, 0, 1);
        lcdPuts (lcdHandle, argv[2]);
    }
    return 0 ;
}
