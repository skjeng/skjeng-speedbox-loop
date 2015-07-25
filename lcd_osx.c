#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <lcd_osx.h>

static void lcd_update(){
	printf("LCD Update");
}

int lcd_init(void){
	printf("LCD Init\n");
	return 0;
}

int led_init(void){
	printf("OSX LED Init");
	return 0;
}

int btn_init(){
	printf("BTN Init");
	return 0;
}

void boardDataUpdate(void){
	printf("Board Data Update");
}

void checkButtons(void){
	printf("check buttons");
}
