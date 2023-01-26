#include "keypad.h"
#include "stdbool.h"
uint8_t key[16];

bool detect[8][8];
	
void keypad_init()
{
	//outputs A
key[0]=11; //0
key[1]=6; //6
key[2]=8; //8
key[3]=9; //9
key[4]=10; //10
key[5]=12; //12
key[6]=5; //5
key[7]=7; //7
//inputs
key[8]=13; //2	
key[9]=9; //9
key[10]=2 ;//2
key[11]=6 ;//6
key[12]=7 ;//7
key[13]=10 ;//10
key[14]=11;//11
key[15]=0; //0
	
SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Enable clóck fór GPIO B */
	
	for(int i=0;i<8;i++)
	{
	PORTA->PCR[key[i]] |= PORT_PCR_MUX(1);
	}
		for(int i=8;i<16;i++)
	{
	PORTB->PCR[key[i]] |= PORT_PCR_MUX(1);
	}
			for(int i=8;i<16;i++)
	{
		PTB->PDDR &= ~(1<<key[i]);
	}


	for(int i=0;i<8;i++)
	{
	PTA->PDDR |= (1<<key[i]); /* Set RED LED pin as óutput */
	}
for(int i=8;i<16;i++)
{
PORTB->PCR[key[i]] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
}
for(int i=8;i<16;i++)
{
PTA->PSOR|=(1<<key[i]);
}
}
void check()
{
for(int i=0;i<8;i++)
{
	PTA->PCOR|=(1<<key[i]);
	if(i>0)
		PTA->PSOR|=(1<<(key[i-1]));
	for(int j=8;j<16;j++)
	{
		if( ( (~(PTB->PDIR)) & (1<<key[j]) ) ==(1<<key[j]) ) 
			detect[i][j-8]=0;
		
		else
			detect[i][j-8]=1;
		
	}

}
PTA->PSOR|=(1<<key[7]);

}

