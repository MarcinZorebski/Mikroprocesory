#include "uart0.h"
#include "buttons.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint32_t i=0;
uint16_t round_count=0;
char rx_buf[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

void UART0_Init(void)
{
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;						
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;						
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(MCGFLLCLK);								
	
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );
	UART0->BDH = 0;
	UART0->BDL =91;
	UART0->C4 &= ~UART0_C4_OSR_MASK;
	UART0->C4 |= UART0_C4_OSR(15);
	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;
	UART0->C2 |= UART0_C2_TE_MASK;		
}

void UART_send(char PGN) {
	PORTB->PCR[1] = PORT_PCR_MUX(2);							
	PORTB->PCR[2] = PORT_PCR_MUX(2);
	
	round_count++;
	sprintf(rx_buf,"%d. %c. ", round_count, PGN);
		for(i=0;rx_buf[i]!=0;i++)
		{
			while(!(UART0->S1 & UART0_S1_TDRE_MASK));
			UART0->D = rx_buf[i];
		}
		
		buttonsInitialize();
		PORTB->PCR[2] |= PORT_PCR_MUX(1); 
		PTB->PDDR &= ~(1<<2);
		PORTB->PCR[2] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
		PTA->PSOR|=(1<<2);
	
};

