#include "clock.h"
#include "lcd1602.h"
#include "keypad.h"
#include "ws2812.h"
#include "cmath"
#include "stdio.h"

#define BTN1 1 //white side
#define BTN2 2 
#define BTN3 7 //black side

#define MIN_3 180
#define MIN_5  300
#define MIN_10 600
#define MIN_15 900
#define MIN_30 1800

#define WHITE 0
#define BLACK 1

uint8_t increment;
uint8_t game_length = 0;
uint8_t active_player = WHITE;
uint16_t start_time;
uint16_t white_time;
uint16_t black_time;
uint16_t minutes = 0;
uint16_t seconds = 0;

bool start_game = false;
struct Board detected_board;

volatile uint8_t num = 0;
extern bool detect[8][8];

char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

//inicialize clock to get welcome screen
void clock_init(void){

	LCD1602_ClearAll();
	LCD1602_SetCursor(0,0);
	LCD1602_Print("3 min.");
	LCD1602_SetCursor(0,1);
	LCD1602_Print("<");
	LCD1602_SetCursor(15,1);
	LCD1602_Print(">");
	start_time = MIN_3;
	increment = 0;
}

//update screen while in menu
void update_lcd(void) {
	LCD1602_ClearAll();
	LCD1602_SetCursor(0,0);
	switch(game_length)
			{
				case 0: 
					LCD1602_Print("3 min.");
					start_time = MIN_3;
					increment = 0;
				break;
				case 1:
					LCD1602_Print("3|2 min.");
					start_time = MIN_3;
					increment = 2;
				break;
				case 2:
					LCD1602_Print("5 min.");
					start_time = MIN_5;
					increment = 0;
					break;
				case 3:
					LCD1602_Print("10 min.");
					start_time = MIN_10;
					increment = 0;
				break;
				case 4:
					LCD1602_Print("15|10 min.");
					start_time = MIN_15;
					increment = 10;
				break;
			 case 5:
					LCD1602_Print("30 min.");
					start_time = MIN_30;
					increment = 0;
				break;
				case 6:
					LCD1602_Print("No time limit");
				break;
			}
			LCD1602_SetCursor(0,1);
			LCD1602_Print("<");
			LCD1602_SetCursor(15,1);
			LCD1602_Print(">");
}

//navigate through game setup menu
bool menu(uint32_t btn) {
	if(btn & (1<<BTN1)){
		if(game_length > 0) {
			game_length--;
		} else {
			game_length = 6;
		}
		update_lcd();
		return false;
	} else if(btn & (1<<BTN2)) {
			active_player = WHITE;
			white_time = black_time = start_time;
			start_game = true;
			LCD1602_SetCursor(0,0);
			LCD1602_Print("WHITE starts");
			return start_game;
	} else if(btn & (1<<BTN3)) {
			if(game_length < 6) {
				game_length++;
			} else {
				game_length = 0;
			}
			update_lcd();
			return false;
	}
}

//update game status: active player, increment, pieces position
bool update_game(uint32_t btn) {
	RTC->SR &= ~RTC_SR_TCE_MASK;
	if(btn & (1<<BTN1)){
		white_time += increment;
		active_player = BLACK;
	}	else if(btn & (1<<BTN3)) {
		black_time += increment;
		active_player = WHITE;
	}
	RTC->TSR = 0;
	RTC->TAR = 0;
	detected_board = check();
	num=0;
	for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{
				
				if(detected_board.pieces[i][j].is_occupied == OCCUPIED)
					setLed(num,0x00,0x00,0x6);
				else
					setLed(num,0x00,0x00,0x00);
					num=num+1;
			}
		}	
	RTC->SR |= RTC_SR_TCE_MASK;
  return start_game;	
}

//update player's time and display it
void update_time(void) {
	if(!active_player) {
		white_time--;
		minutes = floor(white_time / 60);
		seconds = white_time % 60;
		sprintf(display,"WHITE   %u:%02u",minutes,seconds);
	} else {
		black_time--;
		minutes = floor(black_time / 60);
		seconds = black_time % 60;
		sprintf(display,"BLACK   %u:%02u",minutes,seconds);
	}
	if((white_time == 0) | (black_time ==0)) {
		start_game = false;
		// todo: display lose screen
	} else {
		LCD1602_ClearAll();
		LCD1602_SetCursor(0,0);
		LCD1602_Print(display);
		LCD1602_SetCursor(15,1);
		LCD1602_Print(">");
	}
}