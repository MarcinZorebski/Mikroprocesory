#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "stdio.h"
#include "buttons.h"
#include "rtc.h"
#include "tpm.h" 
#include "ws2812.h"
#include "keypad.h"
#include "clock.h"
#include "uart0.h"


int main()
{
	RTCInit();
	LCD1602_Init(); 
	LCD1602_Backlight(TRUE);
	buttonsInitialize();
	TPM0_Init_OC();
	clock_init();
	keypad_init();
	UART0_Init();
	
	while(1);

}
