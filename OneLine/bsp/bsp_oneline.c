#include "bsp_oneline.h"


#if 0

PROTOCOL_SENDMODE Protocol_SendMode;

#else

unsigned char   command= 0x01,commandTmp=0x01;
unsigned char   processSender;
unsigned char   index = 8;
unsigned char   u8Send0Cnt, u8Send1Cnt, u8SendHeaderCnt, u8SendEnderCnt, u8SenderCnt;



#endif

#if 0
unsigned char Data,Rev_Data,Total_Data,Value,StartRevDataFlag,LowLevelFlag;
unsigned int HightLevelTime,LowLevelTime,u8Cnts50ms;
#else
unsigned char u8cnt50ms,u8CntMs;//50ms计时

unsigned char Start_Receive_Data ;//开始接收数据
unsigned char Value,STAT,R_Data;
unsigned char High_Leve_Time,Low_Leve_Time;//高电平时间
unsigned char Total_Data=8;    //总数据长度
unsigned int u16Cnt4hours;
unsigned char  u8shuttime;
unsigned char  u8Data_time;//接收数据的时间，如果0.5S之内没有收到数据，将发热片关闭
unsigned char Flag;
unsigned char Data;

#endif

/**********************************************************************************
*作者:peter
*日期:190401
*描述:IO口输出初始化
*返回:无
***********************************************************************************/
void OneLineOut_Init(void)
{
	P0CON |= 0X01;
	P0PH   &= ~0X01;
}
/**********************************************************************************
*作者:peter
*日期:190401
*描述:IO口输入初始化
*返回:无
***********************************************************************************/
void OneLineIn_Init(void)
{
	P0CON &= ~0X01;
	P0PH   |=  0X01;
}

/**********************************************************************************
*作者:peter
*日期:190401
*描述:数据1处理
*返回:无
***********************************************************************************/
#if 0
static void Data1_Handle(void)
{
	Protocol_SendMode.u16Send1Cnts ++;
	if(Protocol_SendMode.u16Send1Cnts < Data1Time9Cnts)
	{
		WriteHight();
	}
	else
	{
		if(Protocol_SendMode.u16Send1Cnts < Data1Time10Cnts)
		{
			WriteLow();
		}
		else
		{
			Protocol_SendMode.u16Send1Cnts = 0;
			Protocol_SendMode.CommandTemp <<= 1;
			Protocol_SendMode.index --;
		}
	}
}
/**********************************************************************************
*作者:peter
*日期:190401
*描述:数据0处理
*返回:无
***********************************************************************************/
static void Data0_Handle(void)
{
	Protocol_SendMode.u16Send0Cnts ++;
	if(Protocol_SendMode.u16Send0Cnts < Data0Time5Cnts)
	{
		WriteHight();
	}
	else
	{
		if(Protocol_SendMode.u16Send0Cnts < Data0Time10Cnts)
		{
			WriteLow();
		}
		else
		{
			Protocol_SendMode.u16Send0Cnts = 0;
			Protocol_SendMode.CommandTemp <<= 1;
			Protocol_SendMode.index --;
		}
	}
}
/**********************************************************************************
*作者:peter
*日期:190401
*描述:数据处理
*返回:无
***********************************************************************************/
static void Data_Handle(void)
{
	if(Protocol_SendMode.ProcessFlag)
	{
		Protocol_SendMode.u16SendHanderCnts ++;
		if(Protocol_SendMode.u16SendHanderCnts < HeadDataTime)
		{
			WriteLow();
		}
		else
             {
			 	if(Protocol_SendMode.index > 0)
			 	{
					if(Protocol_SendMode.CommandTemp & 0x80)
					{
						Data1_Handle();
					}
					else
					{
						Data0_Handle();
					}
				}
				else
				{
					Protocol_SendMode.u16SendEnderCnts ++;
					WriteLow();
					if(Protocol_SendMode.u16SendEnderCnts == EndDataTime)
					{
						WriteHight();
						Protocol_SendMode.u16SendEnderCnts = 0;
						Protocol_SendMode.u16SendHanderCnts = 0;
						Protocol_SendMode.ProcessFlag = 0;
						Protocol_SendMode.index = 8;
					}
				}
		}
	}
}
#endif
/**********************************************************************************
*作者:peter
*日期:190401
*描述:一串数据发送处理
*返回:无
***********************************************************************************/
void OneLineOut_Handle(void)
{
	#if 0
	Protocol_SendMode.u16SenderCnts ++;
	if(Protocol_SendMode.u16SenderCnts == SerialDataTime)
	{
		Protocol_SendMode.u16SenderCnts = 0;
		Protocol_SendMode.ProcessFlag = 1;
		Protocol_SendMode.CommandTemp = Protocol_SendMode.Command;
	}
	Data_Handle();
	#else
       u8SenderCnt++;

	if (200 == u8SenderCnt) 
	{//每50ms之内发送一次数据 ，即 发送数据所需时间 + 发送间隔 = 50
		u8SenderCnt = 0;
		processSender = 1;
		commandTmp = command;
	}
	if (processSender) 
	{

		//发送信息头
		u8SendHeaderCnt++;
		if (u8SendHeaderCnt < 20) 	 //输出19*0.25=4.75ms低电平
		{
			//SENDER_WRITE_0;
			WriteLow();
		} 
		else 
		{
			if (index > 0) 
			{
				if (commandTmp & 0x80) 
				{//输出数据1
					u8Send1Cnt++;
					if (u8Send1Cnt < 9) 
					{
						WriteHight();
						//SENDER_WRITE_1;   //输出8*0.25=2ms高电平
					}
					else
					{
						if (u8Send1Cnt < 10)   //输出2*0.25=0.5ms低电平
						{
							//SENDER_WRITE_0;
							WriteLow();
						} 
						else
						{//输出数据1操作完成
							u8Send1Cnt = 0;
							commandTmp <<= 1;
							index--;
						}
					}
				}
				else
				{ //输出数据0
					u8Send0Cnt++;
					if (u8Send0Cnt < 5) 
					{//输出了1ms的高电平
						//SENDER_WRITE_1;
						WriteHight();
					} 
					else
					{
						if (u8Send0Cnt < 10)  //输出4*0.25=1ms高电平
						{
							//SENDER_WRITE_0;
							WriteLow();
						} 
						else 
						{ //输出数据0操作完成
							u8Send0Cnt = 0;
							u8Send0Cnt = 0;
							commandTmp <<= 1;
							index--;
						}
					}
				}
			} 
			else 
			{//送数完成，开始输出信息结束符
				u8SendEnderCnt++;
				//SENDER_WRITE_0;//输出低电平
				WriteLow();
				if (24 == u8SendEnderCnt)	   //输出23*0.25=5.75ms低电平后，接着输出高电平，一个byte的数发送完毕
				{
					//SENDER_WRITE_1;
					WriteHight();
					index = 8;
					processSender = 0;
					u8SendHeaderCnt = 0;
					u8SendEnderCnt = 0;
				}
			}
		}
	}


	#endif
}

/**********************************************************************************
*作者:peter
*日期:190401
*描述:一串数据接收处理
*返回:无
***********************************************************************************/
void OneLineIn_Handle(void)
{
	#if 0
	Rev_Data = ONELINE_PIN;
	if((!Rev_Data)&&(!StartRevDataFlag))
	{
		++ u8Cnts50ms;
	}
	if(Rev_Data)
	{
		if((u8Cnts50ms >= 15)&&(u8Cnts50ms <= 22))
		{
			HightLevelTime = 0;
			LowLevelTime = 0;
			u8Cnts50ms= 0;
			Value = 0;
			StartRevDataFlag = 1;
			Total_Data = 8;
		}
		else if(u8Cnts50ms > 22)
		{
			HightLevelTime = 0;
			LowLevelTime = 0;
			u8Cnts50ms= 0;
			Value = 0;
			StartRevDataFlag = 0;
			Total_Data = 8;
		}
		else
		{
			u8Cnts50ms = 0;
		}
	}
	if(StartRevDataFlag)
	{
		if(Total_Data > 0)
		{
			if(Rev_Data)
			{
				LowLevelFlag = 1;
				HightLevelTime ++;
			}
			if((!Rev_Data)&&(LowLevelFlag))
			{
				LowLevelFlag = 0;
				Value <<= 1;
				LowLevelTime ++;
				if((HightLevelTime >= 7)&&(HightLevelTime <= 10))
				{
					HightLevelTime = 0;
					LowLevelTime = 0;
					Value |= 0x01;
					Total_Data --;
				}
				if((HightLevelTime >= 1)&&(HightLevelTime <= 6))
				{
					HightLevelTime = 0;
					LowLevelTime = 0;
					Value &= ~0x01;
					Total_Data --;
				}
				if((HightLevelTime > 16)&&(LowLevelTime>8))
				{
					HightLevelTime = 0;
					LowLevelTime = 0;
					u8Cnts50ms= 0;
					Value = 0;
					StartRevDataFlag = 0;
					Total_Data = 8;
				}
			}
		}
		else
		{
			HightLevelTime = 0;
			LowLevelTime = 0;
			u8Cnts50ms= 0;
			Data = Value;
			StartRevDataFlag = 0;
			Total_Data = 8;
		}
	}
	#else
R_Data=ONELINE_PIN;
if((R_Data==0)&&(Start_Receive_Data==0))  //引导码低电平的时间
{
	++u8cnt50ms;
}
if(R_Data==1)          //电平为高时判断低电平的时间
{
	if((15<=u8cnt50ms)&&(22>=u8cnt50ms))    //时间范围在引导低电平的范围内，开始接收
	{
		Low_Leve_Time=0;
		u8cnt50ms=0;
		High_Leve_Time=0;
		Value=0;
		Total_Data=8;
		Start_Receive_Data=1;
	}
	else if(22<=u8cnt50ms)     //大于 否则不接收
	{
		Low_Leve_Time=0;
		u8cnt50ms=0;
		High_Leve_Time=0;
		Value=0;
		Total_Data=8;
		Start_Receive_Data=0;
	}
	else                       //否则清除计数
	{
		u8cnt50ms=0;
	}	    
}

if(Start_Receive_Data)   //开始接收数据
{
	if(Total_Data>0)    //接收八次
	{ 
		if(R_Data==1)       //高电平的时间
		{
			Flag=1;            //低电平时判断标志
			High_Leve_Time++;   //高电平时间计数
		}
		if((R_Data==0)&&(Flag==1))  //低电平时判断高电平时间，flag标志位只进入一次，																	//只有在下次翻转的时候才能进入判断
		{ 
			Value=Value<<1;             //数据首先左移，八位数据只需移位七次
			Flag=0;                    //清标志
			Low_Leve_Time++;            //低电平时间计数
			if((High_Leve_Time>=7)&&(High_Leve_Time<=10))  //判断码为1
			{
				Low_Leve_Time=0;
				High_Leve_Time=0;
				Value |= 0x01;
				Total_Data--;
			}
			else if((High_Leve_Time>=1)&&(High_Leve_Time<=6))  //判断码为0
			{
				Low_Leve_Time=0;
				High_Leve_Time=0;
				Value &= (~0x01);
				Total_Data--;
		       }
		} 
		if((High_Leve_Time>16)||(Low_Leve_Time>8))      //容错处理，在接收完数据后会进入
		{
			Low_Leve_Time=0;
			High_Leve_Time=0;
			u8cnt50ms=0;
			Start_Receive_Data=0;
			Value=0;
			Total_Data=8;
		}
	}
else   //接收完成，清除相关数据
{
	Low_Leve_Time=0;
	High_Leve_Time=0;
	u8cnt50ms=0;
	Start_Receive_Data=0;
	Data=Value;
	Total_Data=8;
	u8Data_time=0;
	//P00 = ~P00;
}
}
	#endif
	
}






