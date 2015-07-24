#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <unistd.h>
#include <string.h>

#ifdef __APPLE__
	/* Do not include any wiringPi libraries on OS X*/
	#include <lcd_osx.h>
#elif __unix__
	#include <wiringPi.h>
	#include <wiringPiI2C.h>
	#include <wiringSerial.h>
	#include <lcd_odroid.h>
#endif

int main(){
	printf("LCD Panel Control Program\n");
	led_init();
}
