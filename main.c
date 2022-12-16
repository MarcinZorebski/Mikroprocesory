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

//#define DEFAULT_SYSTEM_CLOCK 41943040u

//predefined game lengths
#define MIN_3 180
#define MIN_5  300
#define MIN_10 600
#define MIN_15 900
#define MIN_30 1800

bool no_time_limit=0;
uint8_t prev_choice=9;
uint16_t chosen_time;
uint16_t minutes=0;
uint16_t seconds=0;
uint32_t prev_wynikp0=0;
uint32_t prev_wynikp1=0;
extern bool current_player;
extern bool menu;
extern uint8_t current_choice;
extern uint8_t saved_choice;
extern uint16_t increment0;
extern uint16_t increment1;
extern uint32_t wynikp0;
extern uint32_t wynikp1;

char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};



int main()
{
	RTCInit();
	LCD1602_Init(); 
	LCD1602_Backlight(TRUE);
	buttonsInitialize();

	while(menu)
	{
		DELAY(10);
		if(current_choice!=prev_choice)
		{
			LCD1602_ClearAll();
			LCD1602_SetCursor(0,0);
			switch(current_choice)
			{
				case 0: 
					LCD1602_Print("3 min.");
				break;
				case 1:
					LCD1602_Print("3|2 min.");
				break;
				case 2:
					LCD1602_Print("5 min.");
					break;
				case 3:
					LCD1602_Print("10 min.");
				break;
				case 4:
					LCD1602_Print("15|10 min.");
				break;
			 case 5:
					LCD1602_Print("30 min.");
				break;
				case 6:
					LCD1602_Print("No time limit");
				break;
			}
			LCD1602_SetCursor(0,1);
			LCD1602_Print("<");
			LCD1602_SetCursor(15,1);
			LCD1602_Print(">");
			prev_choice=current_choice;
		}
	}
	
	switch(saved_choice)
		{
			case 0: 
				chosen_time=MIN_3;
			break;
			case 1:
				chosen_time=MIN_3;
			break;
			case 2:
				chosen_time=MIN_5;
			break;
			case 3:
				chosen_time=MIN_10;
			break;
		  case 4:
				chosen_time=MIN_15;
			break;
			case 5:
				chosen_time=MIN_30;
			break;
			case 6:
				no_time_limit=1;
			break;
		}
	wynikp1=wynikp0;
	RTC->SR |= RTC_SR_TCE_MASK; //enable RTC

	if(!no_time_limit)
	{
		while(1)
		{
			
			if(current_player==0)
			{
				wynikp0=chosen_time-(RTC->TSR)+increment0; //calculate left time using current time from RTC
				if(wynikp0!=prev_wynikp0) //update every second
				{
					minutes=floor(wynikp0/60);
					seconds=wynikp0%60;
					sprintf(display,"P1   %u:%02u",minutes,seconds);
					LCD1602_ClearAll();
					LCD1602_SetCursor(0,0);
					LCD1602_Print(display);
					LCD1602_SetCursor(15,1);
					LCD1602_Print(">");
				}
				DELAY(10);
				prev_wynikp0=wynikp0;
			}
			else
			{	
				wynikp1=chosen_time-(RTC->TSR)+increment1;
				if(wynikp1!=prev_wynikp1)
				{
					minutes=floor(wynikp1/60);
					seconds=wynikp1%60;
					sprintf(display,"P2   %u:%02u",minutes,seconds);
					LCD1602_ClearAll();
					LCD1602_SetCursor(0,0);
					LCD1602_Print(display);
					LCD1602_SetCursor(0,1);
					LCD1602_Print("<");
				}
				DELAY(10);
				prev_wynikp1=wynikp1;
			}
		}
	}
}
