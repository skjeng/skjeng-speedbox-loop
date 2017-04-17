//------------------------------------------------------------------------------------------------------------
// ODROID-C 16x2 LED output application.
// Compile: gcc -o led.out led.c -lwiringPi -lwiringPiDev -lpthread
// Run : ./led 1000001 <0 = off, 1 = on>
// Requires user access to gpio. Make udev rules. Total of 7 LEDs.
//------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <unistd.h>
#include <string.h>
#include <time.h>
 
#include <wiringPi.h>

const int ledPorts[] = {
    21, // GPIOX.BIT4(#101)
    22, // GPIOX.BIT3(#100)
    23, // GPIOX.BIT11(#108):PWM_B
    24, // GPIOX.BIT0(#97)
    11, // GPIOX.BIT21(#118)
    26, // GPIOX.BIT2(#99)
    27, // GPIOX.BIT1(#98)
};

#define MAX_LED_CNT sizeof(ledPorts) / sizeof(ledPorts[0])
 
int main (int argc, char *argv[]){
    int i = 0;
    int n = 0;
    if ( argc != 2 ) {
        printf( "usage: %s 0000000", argv[0] );
    } else {
        wiringPiSetup();
        for(i = 0; i < MAX_LED_CNT; i++)    {
            pinMode (ledPorts[i], OUTPUT);
        }
            for(i = 0; i < MAX_LED_CNT; i++) {
                digitalWrite (ledPorts[i], 0);
            }

            for(i = 0; i < strlen(argv[1]); i++){
               n = (int)argv[1][i]-'0';
               digitalWrite(ledPorts[i], n);
            }
    }
    return 0 ;
}
