#include "MKL05Z4.h"                    
#include "buttons.h"
#include "keypad.h"
#include "ws2812.h"
#include "clock.h"
#include "uart0.h"

#define BUT1 1

State game_status = WAITING;
uint8_t press_time;

static const IRQn_Type myPORT_IRQn = 31;


void PORTB_IRQHandler(void)
{	
	if( PORTB->ISFR & (1<<BUT1) ) {
		RTC->SR &= ~RTC_SR_TCE_MASK;
		RTC->TSR = 0;
		RTC->TAR = 0;
		RTC->SR |= RTC_SR_TCE_MASK;
		while((FPTB->PDIR&(1<<BUT1))== 0);		
		press_time = RTC->TSR;
		RTC->SR &= ~RTC_SR_TCE_MASK;
		if(game_status == ONGOING) {
			game_status = update_game(PORTB->ISFR);
		} else if(game_status == WAITING) {
			game_status = menu(press_time);
		} else if(game_status == FINISHED) {
			game_status = update_game(press_time);
		}
		PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK;		
	} 
}


void buttonsInitialize(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK; 				
	PORTB->PCR[BUT1] |= PORT_PCR_MUX(1);     
	PORTB->PCR[BUT1] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK); 
	PORTB->PCR[BUT1] |= PORT_PCR_IRQC(10);		
	PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK;	
	NVIC_ClearPendingIRQ(myPORT_IRQn);				
	NVIC_EnableIRQ(myPORT_IRQn);							
	NVIC_SetPriority (myPORT_IRQn, 3);			
}
