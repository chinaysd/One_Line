#include "bsp_led.h"




void Led_Init(void)
{
	P1CON |= 0X01;
	P1PH   &= ~0X01;
	LED1_SET(0);
}





