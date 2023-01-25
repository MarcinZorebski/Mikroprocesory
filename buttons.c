#include "MKL05Z4.h"                    
#include "buttons.h"
#include "keypad.h"
#include "stdbool.h"
#include "ws2812.h"
#include "clock.h"

#define MENU_OPTION_NUMBER 6 //number of chess timing options in menu
#define BUT1 15
#define BUT2 2
#define BUT3 7

bool game_started = false;

static const IRQn_Type myPORT_IRQn = 31;

void PORTB_IRQHandler(void)
{	
	if( PORTB->ISFR & (1<<BUT1) ) {
		while((FPTB->PDIR&(1<<BUT1))==0);
		if(!game_started) {
			game_started = menu(PORTB->ISFR);
		} else {
			game_started = update_game(PORTB->ISFR);
		}
		PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK;
	} else if( PORTB->ISFR & (1<<BUT2) ) {
		while((FPTB->PDIR&(1<<BUT2))==0);
		if(!game_started) {
			game_started = menu(PORTB->ISFR);
		} else {
			game_started = update_game(PORTB->ISFR);
		}
		PORTB->PCR[BUT2] |= PORT_PCR_ISF_MASK;
	} else if( PORTB->ISFR & (1<<BUT3) ) {
		while((FPTB->PDIR&(1<<BUT3))==0);
		if(!game_started) {
			game_started = menu(PORTB->ISFR);
		} else {
			game_started = update_game(PORTB->ISFR);
		}
		PORTB->PCR[BUT3] |= PORT_PCR_ISF_MASK;
	}
}


void buttonsInitialize(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK; 				
	PORTB->PCR[BUT1] |= PORT_PCR_MUX(1);      	
	PORTB->PCR[BUT2] |= PORT_PCR_MUX(1);      	
	PORTB->PCR[BUT3] |= PORT_PCR_MUX(1);
	PORTB->PCR[BUT1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //pullup enable 
	PORTB->PCR[BUT1] |= PORT_PCR_IRQC(10);		
	PORTB->PCR[BUT2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;			
	PORTB->PCR[BUT2] |= PORT_PCR_IRQC(10);	  
	PORTB->PCR[BUT3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;			
	PORTB->PCR[BUT3] |= PORT_PCR_IRQC(10);
	PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK;
	PORTB->PCR[BUT2] |= PORT_PCR_ISF_MASK;
	PORTB->PCR[BUT3] |= PORT_PCR_ISF_MASK;	
	NVIC_ClearPendingIRQ(myPORT_IRQn);				
	NVIC_EnableIRQ(myPORT_IRQn);							
	NVIC_SetPriority (myPORT_IRQn, 2);			
}
