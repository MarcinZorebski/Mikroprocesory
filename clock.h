/* 
	loibrary description
*/
#include "stdbool.h"
#include "MKL05Z4.h" 

typedef enum {
	WAITING,
	ONGOING,
	FINISHED
} State;

void clock_init(void); // inicialize clock
State menu(uint32_t time); // choose game length
void update_lcd(void); // display actual menu
bool update_game(uint32_t btn); // update player's move
void update_time(void); // update player's time