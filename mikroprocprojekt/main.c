/**************************************************************
 * This file is a part of the PWM Demo (C).                   *
 **************************************************************/
/**
 * @file main.c
 * @author Koryciak
 * @date Nov 2020 
 * @brief File containing the main function. 
 * @ver 0.5
 */
 
#include "frdm_bsp.h" 
#include "tpm.h" 
#include "ws2812.h"
#include "stdbool.h"
#include "keypad.h"
extern bool detect[8][8];

volatile uint8_t num=0;

void delay_ms( int n) {
 int i;
 int j;
for( i = 0 ; i < n; i++)
for(j = 0; j < 3500; j++) {}
}

int main (void) { 
keypad_init();
TPM0_Init_OC();
	
	NVIC_SetPriority(TPM0_IRQn, 2);
NVIC_ClearPendingIRQ(TPM0_IRQn);
NVIC_EnableIRQ(TPM0_IRQn);
	NVIC_DisableIRQ(PORTA_IRQn);
  while(1) {
		check();
		num=0;
for(int i=0;i<8;i++)
	{
		for(int j=7;j>-1;j--)
		{
			
			if(detect[i][j]==0)
			setLed(num,0x00,0x00,0xFF);
			else
			setLed(num,0x00,0x00,0x00);
			num=num+1;
			
		}
	}															/* sleep & wait for interrupt */
	}
}
