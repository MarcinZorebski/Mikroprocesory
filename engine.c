#include "MKL05Z4.h" 
#include "lcd1602.h"
#include "frdm_bsp.h"
#include "string.h"
#include "engine.h"
#include "keypad.h"

struct Board *current_board;

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
		start_board.pieces[0][i].is_occupied = OCCUPIED;
		start_board.pieces[1][i].is_occupied = OCCUPIED;
		start_board.pieces[6][i].is_occupied = OCCUPIED;
		start_board.pieces[7][i].is_occupied = OCCUPIED;
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
	
	
	current_board->pieces[0][0].type = ROOK;
	current_board->pieces[0][1].type = KNIGHT;
	current_board->pieces[0][2].type = BISHOP;
	current_board->pieces[0][3].type = QUEEN;
	current_board->pieces[0][4].type = KING;
	current_board->pieces[0][5].type = BISHOP;
	current_board->pieces[0][6].type = KNIGHT;
	current_board->pieces[0][7].type = ROOK;
	current_board->pieces[7][0].type = ROOK;
	current_board->pieces[7][1].type = KNIGHT;
	current_board->pieces[7][2].type = BISHOP;
	current_board->pieces[7][3].type = QUEEN;
	current_board->pieces[7][4].type = KING;
	current_board->pieces[7][5].type = BISHOP;
	current_board->pieces[7][6].type = KNIGHT;
	current_board->pieces[7][7].type = ROOK;
	
	for(uint8_t i=0; i<8; i++) {
		current_board->pieces[1][i].type = PAWN;
		current_board->pieces[6][i].type = PAWN;
	}
	
	for(uint8_t i=0; i<8; i++) {
		current_board->pieces[0][i].color = WHITE;
		current_board->pieces[1][i].color = WHITE;
	}
	
	for(uint8_t i=0; i<8; i++) {
		current_board->pieces[7][i].color = BLACK;
		current_board->pieces[6][i].color = BLACK;
	}
	
	for(uint8_t i = 0; i<8; i++) {
		current_board->pieces[0][i].is_occupied = OCCUPIED;
		current_board->pieces[1][i].is_occupied = OCCUPIED;
		current_board->pieces[6][i].is_occupied = OCCUPIED;
		current_board->pieces[7][i].is_occupied = OCCUPIED;
	}
	
	
	current_board->column[0] = 'A';
	current_board->column[1] = 'B';
	current_board->column[2] = 'C';
	current_board->column[3] = 'D';
	current_board->column[4] = 'E';
	current_board->column[5] = 'F';
	current_board->column[6] = 'G';
	current_board->column[7] = 'H';
	current_board->row[0] = '1';
	current_board->row[1] = '2';
	current_board->row[2] = '3';
	current_board->row[3] = '4';
	current_board->row[4] = '5';
	current_board->row[5] = '6';
	current_board->row[6] = '7';
	current_board->row[7] = '8';
	return start_board;
}

void analyze(struct Board *detected_board, char *PGN) {
	for(uint8_t i=0; i<8; i++) {
		for(uint8_t j=0; j<8; j++) {
			if((detected_board->pieces[i][j].is_occupied == OCCUPIED) & (current_board->pieces[i][j].is_occupied == EMPTY)) {
				for(uint8_t k=0; k<8; k++) {
					for(uint8_t l=0; l<8; l++) {
						if((detected_board->pieces[i][j].is_occupied == EMPTY) & (current_board->pieces[i][j].is_occupied == OCCUPIED)) {
							detected_board->pieces[i][j].type = current_board->pieces[k][l].type;
							detected_board->pieces[i][j].color = current_board->pieces[k][l].color;
							detected_board->pieces[i][j].type = NONE;		
				
							switch(detected_board->pieces[i][j].type){
							case(NONE):
								PGN = "EEE";
								break;
							case(PAWN):
								PGN[0] = '0';
								PGN[1] = detected_board->row[i];
								PGN[2] = detected_board->column[j];
								break;
							case(ROOK):
								PGN[0] = 'R';
								PGN[1] = detected_board->row[i];
								PGN[2] = detected_board->column[j];
								break;
							case(KNIGHT):
								PGN[0] = 'N';
								PGN[1] = detected_board->row[i];
								PGN[2] = detected_board->column[j];
								break;
							case(BISHOP):
								PGN[0] = 'B';
								PGN[1] = detected_board->row[i];
								PGN[2] = detected_board->column[j];
								break;
							case(QUEEN):
								PGN[0] = 'Q';
								PGN[1] = detected_board->row[i];
								PGN[2] = detected_board->column[j];
								break;
							case(KING):
								PGN[0] = 'K';
								PGN[1] = detected_board->row[i];
								PGN[2] = detected_board->column[j];
								break;
							}
						}
					}
				}
			}
		}
	}
	current_board = detected_board;
}
