#include "MKL05Z4.h" 
#include "rtc.h"

void RTCInit(void)
{
	SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
	RTC->CR|=RTC_CR_SWR_MASK; //softwarereset
	RTC->CR&=~RTC_CR_SWR_MASK;
	SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK; //oscillator selection
	RTC->CR|=RTC_CR_OSCE_MASK;
	RTC->TSR = 0; 
	//RTC->LR &= ~RTC_LR_LRL_MASK;
	//RTC->TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);
	//RTC->LR |= RTC_LR_TCL_MASK;
	//RTC->LR |= RTC_LR_LRL_MASK;
	
	
}
