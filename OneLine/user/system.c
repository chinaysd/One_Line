#include "system.h"


TIMEOUT_PARA TimeOut_Para[2];
//extern PROTOCOL_SENDMODE Protocol_SendMode; 
extern unsigned char Data;




void System_Init(void)
{
	 //Protocol_SendMode.index = 8;
	 //Protocol_SendMode.Command = 0x01;
	 Led_Init();
	 //OneLineOut_Init();
	 OneLineIn_Init();
	 TimeOutDet_Init();
	 Timer_Init();
}

void System_Handle(void)
{
	static unsigned char Cnts;
	if(TimeOutDet_Check(&TimeOut_Para[0]))
	{
		TimeOut_Record(&TimeOut_Para[0],OnLine_Time);
		++ Cnts;
              if(Cnts & 0x01)
		{
	            //LED1_SET(1);		
		}
	       else
	       {
		     //LED1_SET(0);
		 }
	}
	if(Data == 0x01)
	{
		LED1_SET(1);	
	}
}


