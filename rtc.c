#include "MKL05Z4.h" 
#include "rtc.h"
#include "clock.h"


void RTCInit(void)
{
	SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
	RTC->CR|=RTC_CR_SWR_MASK; //softwarereset
	RTC->CR&=~RTC_CR_SWR_MASK;
	SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK; //oscillator selection
	RTC->CR|=RTC_CR_OSCE_MASK;
	RTC->TSR = 0;
	RTC->TAR = 0;;
	NVIC_SetPriority(RTC_IRQn, 3);
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_EnableIRQ(RTC_IRQn);
	
}


void RTC_IRQHandler(void) {
	RTC->TAR = RTC->TSR;
	update_time();
}
