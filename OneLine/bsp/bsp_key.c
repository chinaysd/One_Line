#include "bsp_key.h"






void Key_Init(void)
{
       P2CON &= ~0X02;
	P2PH    |= 0X02;
}

unsigned char Read_Key(unsigned int GPIO_PORT,unsigned char GPIO_PIN)
{
	if(GPIO_PORT == P2)
	{
		if(0 == GPIO_PIN)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}



