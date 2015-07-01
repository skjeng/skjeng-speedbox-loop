#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <unistd.h>
#include <string.h>
#include <time.h>
 
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <lcd.h>

//------------------------------------------------------------------------------------------------------------
//
// LCD:
//
//------------------------------------------------------------------------------------------------------------
#define LCD_ROW             2   // 16 Char
#define LCD_COL             16  // 2 Line
#define LCD_BUS             4   // Interface 4 Bit mode
#define LCD_UPDATE_PERIOD   300 // 300ms
 
static unsigned char lcdFb[LCD_ROW][LCD_COL] = {0, };
 
static int lcdHandle  = 0;
static int lcdDispPos = 0;
 
#define PORT_LCD_RS     7   // GPIOY.BIT3(#83)
#define PORT_LCD_E      0   // GPIOY.BIT8(#88)
#define PORT_LCD_D4     2   // GPIOX.BIT19(#116)
#define PORT_LCD_D5     3   // GPIOX.BIT18(#115)
#define PORT_LCD_D6     1   // GPIOY.BIT7(#87)
#define PORT_LCD_D7     4   // GPIOX.BIT7(#104)

const unsigned char screenInit[LCD_ROW][LCD_COL]    = { "CosyPerf Loading", ".......v1.......", };
unsigned char screen[LCD_ROW][LCD_COL]   = { "                ", "                ", };

// 16 characters, one EOL
char a[16];
char b[16];
//---------------
// Button:
//---------------
#define PORT_BUTTON1    5   // GPIOX.BIT5(#102)
#define PORT_BUTTON2    6   // GPIOX.BIT6(#103)
//---------------
// LED:
//---------------
static int ledPos = 0;
 
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
 
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//
// LCD Update Function:
//
//------------------------------------------------------------------------------------------------------------
static void lcd_update ()
{
    int i, j;
 
    memset((void *)&lcdFb, ' ', sizeof(lcdFb));

    strncpy(&lcdFb[0][0], &a[0], LCD_COL);
    strncpy(&lcdFb[1][0], &b[0], LCD_COL);
 
    for(i = 0; i < LCD_ROW; i++)    {
        lcdPosition (lcdHandle, 0, i);
        for(j = 0; j < LCD_COL; j++) {
            lcdPutchar(lcdHandle, lcdFb[i][j]);
        };
    }
}
 
//------------------------------------------------------------------------------------------------------------
//
// system init
//
//------------------------------------------------------------------------------------------------------------
int lcd_init(void){
 
    // LCD Init   
    lcdHandle = lcdInit (LCD_ROW, LCD_COL, LCD_BUS,
                         PORT_LCD_RS, PORT_LCD_E,
                         PORT_LCD_D4, PORT_LCD_D5, PORT_LCD_D6, PORT_LCD_D7, 0, 0, 0, 0);
 
    if(lcdHandle < 0)   {
        fprintf(stderr, "%s : lcdInit failed!\n", __func__);    return -1;
    }
 
    return  0;
 }

int led_init(void){
    int i;
    // GPIO Init(LED Port ALL Output)
    for(i = 0; i < MAX_LED_CNT; i++)    {
        pinMode (ledPorts[i], OUTPUT);
    }
}

int btn_init(void){
   pinMode (PORT_BUTTON1, INPUT);    pullUpDnControl (PORT_BUTTON1, PUD_UP);
    pinMode (PORT_BUTTON2, INPUT);    pullUpDnControl (PORT_BUTTON2, PUD_UP);
}
 
//------------------------------------------------------------------------------------------------------------
//
// board data update
//
//------------------------------------------------------------------------------------------------------------
void boardDataUpdate(void)
{
    int i;
 
    //  LED Control
    for(i = 0; i < MAX_LED_CNT; i++){
        digitalWrite (ledPorts[i], 0); // LED All Clear
    };
 
}

void checkButtons(void)
{
   if(!digitalRead (PORT_BUTTON1)){
   printf("BUTTON1");
   }
   if(!digitalRead (PORT_BUTTON2)){
   printf("BUTTON2");
   }
}
 
//------------------------------------------------------------------------------------------------------------
//
// Start Program
//
//------------------------------------------------------------------------------------------------------------
int main (int argc, char *argv[])
{   
    wiringPiSetup ();
    if (argc > 1){
		    if (lcd_init() < 0)
		    {
			fprintf (stderr, "%s: LCD Init failed\n", __func__);
			return -1;
		    }
	    strncpy(a, argv[1], 16);
	    strncpy(b, argv[2], 16);
	    // ISSUE: Does only handle 15 characters?!
	    printf("line1=%s\n", a);
	    printf("line2=%s\n", b);
            lcd_update();
    }
 
    if (btn_init() < 0)
    {
        fprintf (stderr, "%s: BTN Init failed\n", __func__);
        return -1;
    }
    
    if (led_init() < 0)
    {
        fprintf (stderr, "%s: LED Init failed\n", __func__);
        return -1;
    }
    checkButtons(); 
    int timer = 0;
    timer = millis () + LCD_UPDATE_PERIOD;
    boardDataUpdate();
    return 0 ;
}
 
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
 
