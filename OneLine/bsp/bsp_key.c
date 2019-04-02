#include "bsp_key.h"


extern TIMEOUT_PARA TimeOut_Para[2];

unsigned char KeyDebounceFlag;


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
			#if 0
			if(!KeyDebounceFlag)
			{
				KeyDebounceFlag = 1;
				TimeOut_Record(&TimeOut_Para[0],KeyDebounceTime);
			}
		      if(TimeOutDet_Check(&TimeOut_Para[0]))
		      {
				return 1;
		      }
		      else
		      {
			  	KeyDebounceFlag = 0;
			  	return 0;
		      }
		      #else
		      return 1;
		      #endif
		}
		else
		{
			KeyDebounceFlag = 0;
			return 0;
		}
	}
	else
	{
		KeyDebounceFlag = 0;
		return 0;
	}
}



