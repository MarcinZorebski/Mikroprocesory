#include "clock.h"
#include "lcd1602.h"
#include "ws2812.h"
#include "cmath"
#include "stdio.h"
#include "engine.h"
#include "keypad.h"

#define BTN1 1 

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
int16_t white_time;
int16_t black_time;
uint16_t minutes = 0;
uint16_t seconds = 0;
bool first_round = false;

State game_state = WAITING;
struct Board detected_board;

volatile uint8_t num = 0;

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
State menu(uint32_t time) {
	if(time > 3) {
		if(game_state == FINISHED) {
			game_state = WAITING;
			game_length = 0;
			update_lcd();
			return game_state;
		} else {
			active_player = WHITE;
			white_time = black_time = start_time;
			game_state = ONGOING;
			first_round = true;
			return game_state;
		}
	} else {
		if(game_length < 6) {
			game_length++;
		} else {
			game_length = 0;
		}
		update_lcd();
		game_state = WAITING;
		return game_state;
	}
}

//update game status: active player, increment, pieces position
bool update_game(uint32_t btn) {
	RTC->SR &= ~RTC_SR_TCE_MASK;
	if(active_player == WHITE) {
		white_time += increment;
		active_player = BLACK;
	} else {
		black_time += increment;
		active_player = WHITE;
	}
	RTC->TSR = 0;
	RTC->TAR = 0;
	detected_board = check();
	num=0;
	for(int i=0;i<8;i++)
		{
			for(int j=7;j>-1;j--)
			{
				
				if(detected_board.pieces[i][j].is_occupied == OCCUPIED)
					setLed(num,0x00,0x00,0x6);
				else
					setLed(num,0x00,0x00,0x00);
					num=num+1;
			}
		}	
	RTC->SR |= RTC_SR_TCE_MASK;
  return game_state;	
}

//update player's time and display it
void update_time(void) {
	if(active_player == WHITE) {
		if(first_round){
			sprintf(display,"WHITE starts");
			first_round = false;
		} else {
			white_time--;
			minutes = floor(white_time / 60);
			seconds = white_time % 60;
			sprintf(display,"WHITE   %u:%02u",minutes,seconds);
		}
	} else {
		black_time--;
		minutes = floor(black_time / 60);
		seconds = black_time % 60;
		sprintf(display,"BLACK   %u:%02u",minutes,seconds);
	}
	if((white_time < 0) | (black_time < 0)) {
		game_state = FINISHED;
		sprintf(display, "KONIEC CZASU");
	}
	LCD1602_ClearAll();
	LCD1602_SetCursor(0,0);
	LCD1602_Print(display);
	LCD1602_SetCursor(15,1);
	LCD1602_Print(">");

}