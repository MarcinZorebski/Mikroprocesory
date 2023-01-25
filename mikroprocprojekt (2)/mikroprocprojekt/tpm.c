

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
//static uint16_t tpm1Diff = 0;
//static uint16_t tpm1New = 0;
//static uint16_t tpm1Old = 0;
//static uint8_t tpm0Enabled = 0;
 uint8_t counter=0;
 uint8_t internal_counter=0;
	bool nextmove=false;

void TPM0_Init_OC(void) {
		
 SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		// ToDo 3.1.1: Enable TPM0 mask in SCGC6 register
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  // ToDo 3.1.1: Choose MCGFLLCLK clock source

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // ToDo 3.1.2: Connect port B to clock
	PORTB->PCR[8] = PORT_PCR_MUX(2);		// ToDo 3.1.2: Set multiplekser to TPM0 for PTB9, get channel number (page 148 of the Reference Manual)

	TPM0->SC |= TPM_SC_PS(0);  					// ToDo 3.1.3: Set prescaler to 128
	TPM0->SC |= TPM_SC_CMOD(1);					// ToDo 3.1.4: For TMP0, select the internal input clock source

	TPM0->CNT=0;
	TPM0->MOD = 16; 										// ToDo 3.1.5: Set modulo value to maximum value from slider
	

	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		/* up counting */
	TPM0->CONTROLS[3].CnSC |= ( TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK );
	TPM0->CONTROLS[3].CnSC &=((~TPM_CnSC_MSA_MASK) & (~TPM_CnSC_ELSA_MASK));/* set Output on match, clear Output on reload */ 
	TPM0->SC |= TPM_SC_TOIE_MASK;
	TPM0->CONTROLS[3].CnSC &= ~TPM_CnSC_CHIE_MASK;
	TPM0->CONTROLS[3].CnV = 0; 				// ToDo 3.1.7: Set starting value to 50
	//tpm0Enabled = 1;  /* set local flag */
	if((tab[counter]&(1<<(23-internal_counter)))==1<<(23-internal_counter))
		nextmove=true;
	else
		nextmove=false;
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
