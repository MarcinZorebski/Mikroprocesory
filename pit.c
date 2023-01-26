/*----------------------------------------------------------------------------
 *      Name:    pit.c
 *
 *      Purpose: Microprocessors Laboratory 2
 *      
 *      Author: Pawel Russek AGH University of Science and Technology
 *---------------------------------------------------------------------------*/
 
#include "MKL05Z4.h"                    //Device header
#include "pit.h"												//Declarations

//static const IRQn_Type myPIT_IRQn = PIT_IRQn;  /*-1 is not a correct value */	/* ToDo 6.1: Define interrupt (IRQ) number for PIT. Check MKL05Z4.h or find in NVIC section of KL05 Sub-Family Reference Manual - NVIC section */ 

 /*----------------------------------------------------------------------------
  Function that initializes PIT timer
 *----------------------------------------------------------------------------*/
void pitInitialize(unsigned period) {

	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; /* ToDo 6.2 Enable PIT mask in SCGC6 register */
	
	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; /* ToDo 6.3 Disable MDIS mask in MCR register */
	PIT->MCR |= PIT_MCR_FRZ_MASK;   /* ToDo 6.4 Enable FRZ mask in MCR register */
	
	// Initialize PIT0 to count down from argument 
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);

	// No chaining
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;

}


void startPIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void stopPIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}


