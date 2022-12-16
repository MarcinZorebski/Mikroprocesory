#include "MKL05Z4.h"                    
#include "buttons.h"

#define MENU_OPTION_NUMBER 6 //number of chess timing options in menu

uint8_t current_player=0;
uint8_t current_choice=0;
uint8_t saved_choice=0;
uint8_t menu=1;
uint16_t temptime0=0;
uint16_t temptime1=0;
uint16_t increment0=0;
uint16_t increment1=0;
uint32_t wynikp0=0;
uint32_t wynikp1=0;

typedef enum{ BUT1 = 1, BUT2 = 2, BUT3 = 7 } ButtonType;
static const IRQn_Type myPORT_IRQn = 31;

void PORTB_IRQHandler(void)
{	
	if( PORTB->ISFR & (1 << BUT1) ) //if button 1 pressed
	{       
    while((FPTB->PDIR&(1<<BUT1))==0);			//wait for release
		
		if(menu) //change options while in menu
		{
			if(current_choice>0)
			current_choice=current_choice-1;
		}			
		else //change players while in timer mode 
		{
			if(current_player==1)
			{
				temptime1=RTC->TSR; //read last players time in seconds
				current_player=0; //change player
				RTC->SR &= ~RTC_SR_TCE_MASK; //disable RTC
	
				if(saved_choice==1) 
					increment1=increment1+2;
				else if(saved_choice==4)
					increment1=increment1+10;	
				RTC->TSR = temptime0; //load current players time into RTC 

				RTC->SR |= RTC_SR_TCE_MASK; //enable RTC
			}
		}
		DELAY(50);
		PORTB->PCR[BUT1] |= PORT_PCR_ISF_MASK; //clear interrupt flag
	}
	
	if( PORTB->ISFR & (1 << BUT2) )
	{          
    while((FPTB->PDIR&(1<<BUT2))==0);				
		if(menu)
		{
			saved_choice=current_choice;
			menu=0;
		}
		DELAY(50);
		PORTB->PCR[BUT2] |= PORT_PCR_ISF_MASK;
  }
	
	if( PORTB->ISFR & (1 << BUT3) )
	{          
    while((FPTB->PDIR&(1<<BUT3))==0);				
		if(menu)
		{
			if(current_choice<MENU_OPTION_NUMBER)
				current_choice=current_choice+1;
		}
		else
		{
			if(current_player==0)
			{
				temptime0=RTC->TSR;
				current_player=1;
				RTC->SR &= ~RTC_SR_TCE_MASK;
				if(saved_choice==1)
					increment0=increment0+2;
				else if(saved_choice==4)
					increment0=increment0+10;	
	
				RTC->TSR = temptime1;

				RTC->SR |= RTC_SR_TCE_MASK;
			}
		}
		DELAY(50);
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
	NVIC_ClearPendingIRQ(myPORT_IRQn);				
	NVIC_EnableIRQ(myPORT_IRQn);							
	NVIC_SetPriority (myPORT_IRQn, 3);			
}
