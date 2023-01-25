#include "keypad.h"


uint8_t key[16];

uint8_t detect[8][8];



	
void keypad_init()
{
	//outputs
	key[0]=9; //9
	key[1]=0; //0
	key[2]=8; //8
	key[3]=6;//6
	key[4]=7; //7
	key[5]=5; //5
	key[6]=12; //12
	key[7]=10; //10
	//inputs
	key[8]=1; //1	
	key[9]=9; //9
	key[10]=5 ;//5
	key[11]=6 ;//6
	key[12]=13 ;//13
	key[13]=10 ;//10
	key[14]=11;//11
	key[15]=0; //0
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Enable clóck fór GPIO B */
	
	for(int i=0;i<8;i++) {
		PORTA->PCR[key[i]] |= PORT_PCR_MUX(1);
	}
	
	for(int i=8;i<16;i++) {
		PORTB->PCR[key[i]] |= PORT_PCR_MUX(1);
	}
	
	for(int i=8;i<16;i++) {
		PTB->PDDR &= ~(1<<key[i]);
	}


	for(int i=0;i<8;i++) {
		PTA->PDDR |= (1<<key[i]);
	}
	
	for(int i=8;i<16;i++) {
		PORTB->PCR[key[i]] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	}
	
	for(int i=8;i<16;i++) {
		PTA->PSOR|=(1<<key[i]);
	}
}

	
struct Board check() {
	struct Board detected_board;
	for(int i=0;i<8;i++) {
		PTA->PCOR|=(1<<key[i]);
		if(i>0)
			PTA->PSOR|=(1<<(key[i-1]));
		for(int j=0;j<8;j++) {
			if( ( ~(PTB->PDIR) & (1<<key[j+8]) ) == (1<<key[j]) ) {
				detected_board.pieces[i][j].is_occupied = OCCUPIED;
			} else {
				detected_board.pieces[i][j].is_occupied = EMPTY;
			}
		}
	}
	PTA->PSOR|=(1<<key[7]);
	return detected_board;
}

