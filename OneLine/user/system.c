#include "system.h"


TIMEOUT_PARA TimeOut_Para[2];

extern PROTOCOL_SENDMODE Protocol_SendMode; 
extern PROTOCOL_REVMODE  Protocol_RevMode;

unsigned char KeyPressFlag;
unsigned char KeyPressRevFlag;

unsigned char Rev_Flag;


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
	#if 0
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
	#endif
	
       #if 1
	if(Read_Key(KEY_PORT,KEY_PIN))
	{
		if(!KeyPressFlag)
		{
			KeyPressFlag = 1;
			KeyPressRevFlag = 1;
			OneLineOut_Init();
			Protocol_SendMode.Command = 0x02;
			TimeOut_Record(&TimeOut_Para[0],OnLine_Time);
		}
		if(TimeOutDet_Check(&TimeOut_Para[0]))
		{
			OneLineIn_Init();
		}
	}
	else
	{
		if(KeyPressFlag)
		{
			KeyPressFlag = 0;
			KeyPressRevFlag = 0;
		      Protocol_SendMode.Command = 0x00;	
		}
	}
	switch (Protocol_RevMode.Data)
	{
		case 0x10:
			        break;
		case 0x20:
			        #if 1
			        if(KeyPressRevFlag)
			        {
					KeyPressRevFlag = 0;
					++ Cnts;
					if(Cnts & 0x01)
					{
					     LED1_SET(1);	
					}
					else
					{
					     LED1_SET(0);
					}
				  }
				  #else
                             LED1_SET(1);
				  #endif
			        break;
		case 0x40:
			        
			        break;
		case 0x80:
			        
			        break;
		default:
			       //LED1_SET(0);
			        break;
	}
       #else
       switch (Protocol_RevMode.Data)
       {
	   	case 0x01:
			       
			       break;
	       case 0x02:
		   	      if(!Rev_Flag)
		   	      {
				  	Rev_Flag = 1;
					LED1_SET(1);
				      OneLineOut_Init();
				      Protocol_SendMode.Command = 0x20;
				      TimeOut_Record(&TimeOut_Para[0],OnLine_Time);
				}
	   	             break;
		case 0x04:
			       
			       break;
	       case 0x08:
		   	       
		   	       break;
		default:
			break;
	 }
	 if(TimeOutDet_Check(&TimeOut_Para[0]))
	 {
	 	TimeOut_restart(&TimeOut_Para[0]);
	 	LED1_SET(0);
		OneLineIn_Init();
		Rev_Flag = 0;
	 }
	#endif
}


