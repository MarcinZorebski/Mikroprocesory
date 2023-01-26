#include "MKL05Z4.h" 
#include "lcd1602.h"
#include "frdm_bsp.h"
#include "string.h"
#include "engine.h"
#include "keypad.h"

struct Board current_board;


struct Board generate_starting_board() { 
	struct Board start_board;
	start_board.pieces[0][0].type = ROOK;
	start_board.pieces[0][1].type = KNIGHT;
	start_board.pieces[0][2].type = BISHOP;
	start_board.pieces[0][3].type = QUEEN;
	start_board.pieces[0][4].type = KING;
	start_board.pieces[0][5].type = BISHOP;
	start_board.pieces[0][6].type = KNIGHT;
	start_board.pieces[0][7].type = ROOK;
	start_board.pieces[7][0].type = ROOK;
	start_board.pieces[7][1].type = KNIGHT;
	start_board.pieces[7][2].type = BISHOP;
	start_board.pieces[7][3].type = QUEEN;
	start_board.pieces[7][4].type = KING;
	start_board.pieces[7][5].type = BISHOP;
	start_board.pieces[7][6].type = KNIGHT;
	start_board.pieces[7][7].type = ROOK;
	
	for(uint8_t i=0; i<8; i++) {
		start_board.pieces[1][i].type = PAWN;
		start_board.pieces[6][i].type = PAWN;
	}
	
	for(uint8_t i=0; i<8; i++) {
		start_board.pieces[0][i].color = WHITE;
		start_board.pieces[1][i].color = WHITE;
	}
	
	for(uint8_t i=0; i<8; i++) {
		start_board.pieces[7][i].color = BLACK;
		start_board.pieces[6][i].color = BLACK;
	}
	
	for(uint8_t i = 0; i<8; i++) {
		start_board.pieces[0][i].is_occupied = 1;
		start_board.pieces[1][i].is_occupied = 1;
		start_board.pieces[6][i].is_occupied = 1;
		start_board.pieces[7][i].is_occupied = 1;
	}
	
	
	start_board.column[0] = 'A';
	start_board.column[1] = 'B';
	start_board.column[2] = 'C';
	start_board.column[3] = 'D';
	start_board.column[4] = 'E';
	start_board.column[5] = 'F';
	start_board.column[6] = 'G';
	start_board.column[7] = 'H';
	start_board.row[0] = '1';
	start_board.row[1] = '2';
	start_board.row[2] = '3';
	start_board.row[3] = '4';
	start_board.row[4] = '5';
	start_board.row[5] = '6';
	start_board.row[6] = '7';
	start_board.row[7] = '8';
	current_board = start_board;	
	return start_board;
}



char analyze(struct Board detected_board) {
	if
	
}
