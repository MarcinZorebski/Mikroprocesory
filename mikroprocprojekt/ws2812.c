#include "ws2812.h"

uint32_t tab [64];

void setLed(uint8_t lednumber, uint32_t red, uint32_t green, uint32_t blue)
{
	tab[lednumber]=(green<<16)|(red<<8)|(blue);
}

