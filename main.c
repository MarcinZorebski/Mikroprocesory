#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "cmath"
#include "stdbool.h"
#include "buttons.h"
#include "rtc.h"
#include "tpm.h" 
#include "ws2812.h"
#include "stdbool.h"
#include "keypad.h"
#include "clock.h"

//#define DEFAULT_SYSTEM_CLOCK 41943040u


int main()
{
	RTCInit();
	LCD1602_Init(); 
	LCD1602_Backlight(TRUE);
	buttonsInitialize();
	TPM0_Init_OC();
	clock_init();
	keypad_init();
	
	while(1);

}
