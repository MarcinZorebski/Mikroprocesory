#include "MKL05Z4.h"
//----------------------------------------------
// SIM_SOPT2
//----------------------------------------------
// Zr�dlo zegara
//
#define CLK_DIS 					0x00
#define MCGFLLCLK 				0x01
#define OSCERCLK					0x02
#define MCGIRCLK					0x03

void UART0_Init(void);
void UART_send(char PGN);