#include "MKL05Z4.h"
#include	"pit.h"



void PIT_Init(void)
{
	uint32_t tsv0;
		uint32_t tsv1;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;			// Włączenie sygnału zegara do modułu PIT
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;				// Włączenie modułu PIT
	//tsv=BUS_CLOCK;												// Przerwanie co 1s
									// Przerwanie co 100ms
	//tsv=BUS_CLOCK/100;									// Przerwanie co 10ms
	PIT->CHANNEL[1].TCTRL = PIT_TCTRL_CHN_MASK;
		tsv0=0xFFFFFFFF;
		tsv1=0x00000001;
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(tsv0);	
	PIT->CHANNEL[1].LDVAL = PIT_LDVAL_TSV(tsv1);
	PIT->CHANNEL[1].TCTRL = PIT_TCTRL_TEN_MASK;	// Załadowanie wartości startowej
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK;	
	// Załadowanie wartości startowej
	// Odblokowanie przerwania i wystartowanie licznika
	//NVIC_ClearPendingIRQ(PIT_IRQn);
	//NVIC_EnableIRQ(PIT_IRQn);	
}
