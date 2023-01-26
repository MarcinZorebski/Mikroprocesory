#include "tpm.h"
#include "ws2812.h"
#include "stdbool.h"
/******************************************************************************\
* Private definitions
\******************************************************************************/
/******************************************************************************\
* Private prototypes
\******************************************************************************/
void TPM1_IRQHandler(void);
/******************************************************************************\
* Private memory declarations
\******************************************************************************/

uint8_t counter=0;
uint8_t internal_counter=0;
bool nextmove=false;

void TPM0_Init_OC(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 
	PORTB->PCR[8] = PORT_PCR_MUX(2);		

	TPM0->SC |= TPM_SC_PS(0);  					
	TPM0->SC |= TPM_SC_CMOD(1);					

	TPM0->CNT=0;
	TPM0->MOD = 16; 										
	

	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		
	TPM0->CONTROLS[3].CnSC |= ( TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK );
	TPM0->CONTROLS[3].CnSC &=((~TPM_CnSC_MSA_MASK) & (~TPM_CnSC_ELSA_MASK));
	TPM0->SC |= TPM_SC_TOIE_MASK;
	TPM0->CONTROLS[3].CnSC &= ~TPM_CnSC_CHIE_MASK;
	TPM0->CONTROLS[3].CnV = 0; 				
	if((tab[counter]&(1<<(23-internal_counter)))==1<<(23-internal_counter))
		nextmove=true;
	else
		nextmove=false;
	
	NVIC_SetPriority(TPM0_IRQn, 2);
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);
}



void TPM0_IRQHandler(void) {


	if(counter<64){
			TPM0->MOD = 160;
		if(nextmove)
		{
			TPM0->CONTROLS[3].CnV=100;
		}
		else 
		{
			TPM0->CONTROLS[3].CnV=12;
		}
		
	internal_counter++;
		
	if(internal_counter==24)
	{
		counter=counter+1;
		internal_counter=0;
	}
}
	else
	{
		TPM0->MOD = 0x6000;
		TPM0->CONTROLS[3].CnV=0;
    TPM0->CNT=0;
		counter=0;
		internal_counter=0;
	}
	if((tab[counter]&(1<<(23-internal_counter)))==1<<(23-internal_counter))
		nextmove=true;
	else
		nextmove=false;
	TPM0->SC |= TPM_SC_TOF_MASK;
	NVIC_ClearPendingIRQ(TPM0_IRQn);
}
