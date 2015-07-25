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

char a[16];
char b[16];

int main(int argc, char *argv[]){
	if (argc > 1){
		if (lcd_init() < 0){
			printf("Failed init");
			return -1;
		}
		strncpy(a, argv[1], 16);
		strncpy(b, argv[2], 16);
		printf("line1=%s\n", a);
		printf("line2=%s\n", b);
	} else {
		printf("Too few arguments");
	}

	return 0;
}
