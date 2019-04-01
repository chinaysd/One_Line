#include "system.h"


TIMEOUT_PARA TimeOut_Para[2];

extern PROTOCOL_SENDMODE Protocol_SendMode; 
extern PROTOCOL_REVMODE  Protocol_RevMode;

unsigned char KeyPressFlag,Flag;


void System_Init(void)
{
	 Protocol_SendMode.index = 8;
	 Protocol_SendMode.Command = 0x00;
	 Led_Init();
	 //OneLineOut_Init();
	 OneLineIn_Init();
	 Key_Init();
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
			//OneLineOut_Init();
		}
	       else
	       {
		   	//OneLineIn_Init();
		}
	}
	if(Read_Key(KEY_PORT,KEY_PIN))
	{
		if(!KeyPressFlag)
		{
			KeyPressFlag = 1;
			OneLineOut_Init();
			Protocol_SendMode.Command = 0x01;
		}
	}
	else
	{
		if(KeyPressFlag)
		{
		      Protocol_SendMode.Command = 0x00;	
		      KeyPressFlag = 0;
		}
	}
	if(Protocol_RevMode.Data == 0x01)
	{
		LED1_SET(1);	
		Flag = 1;
	}
	else
	{
		LED1_SET(0);	
		if(Flag)
		{
			Flag = 0;
			OneLineIn_Init();
		}
	}
}


