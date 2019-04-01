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
unsigned char u8cnt50ms,u8CntMs;//50ms��ʱ

unsigned char Start_Receive_Data ;//��ʼ��������
unsigned char Value,STAT,R_Data;
unsigned char High_Leve_Time,Low_Leve_Time;//�ߵ�ƽʱ��
unsigned char Total_Data=8;    //�����ݳ���
unsigned int u16Cnt4hours;
unsigned char  u8shuttime;
unsigned char  u8Data_time;//�������ݵ�ʱ�䣬���0.5S֮��û���յ����ݣ�������Ƭ�ر�
unsigned char Flag;
unsigned char Data;

#endif

/**********************************************************************************
*����:peter
*����:190401
*����:IO�������ʼ��
*����:��
***********************************************************************************/
void OneLineOut_Init(void)
{
	P0CON |= 0X01;
	P0PH   &= ~0X01;
}
/**********************************************************************************
*����:peter
*����:190401
*����:IO�������ʼ��
*����:��
***********************************************************************************/
void OneLineIn_Init(void)
{
	P0CON &= ~0X01;
	P0PH   |=  0X01;
}

/**********************************************************************************
*����:peter
*����:190401
*����:����1����
*����:��
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
*����:peter
*����:190401
*����:����0����
*����:��
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
*����:peter
*����:190401
*����:���ݴ���
*����:��
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
*����:peter
*����:190401
*����:һ�����ݷ��ʹ���
*����:��
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
	{//ÿ50ms֮�ڷ���һ������ ���� ������������ʱ�� + ���ͼ�� = 50
		u8SenderCnt = 0;
		processSender = 1;
		commandTmp = command;
	}
	if (processSender) 
	{

		//������Ϣͷ
		u8SendHeaderCnt++;
		if (u8SendHeaderCnt < 20) 	 //���19*0.25=4.75ms�͵�ƽ
		{
			//SENDER_WRITE_0;
			WriteLow();
		} 
		else 
		{
			if (index > 0) 
			{
				if (commandTmp & 0x80) 
				{//�������1
					u8Send1Cnt++;
					if (u8Send1Cnt < 9) 
					{
						WriteHight();
						//SENDER_WRITE_1;   //���8*0.25=2ms�ߵ�ƽ
					}
					else
					{
						if (u8Send1Cnt < 10)   //���2*0.25=0.5ms�͵�ƽ
						{
							//SENDER_WRITE_0;
							WriteLow();
						} 
						else
						{//�������1�������
							u8Send1Cnt = 0;
							commandTmp <<= 1;
							index--;
						}
					}
				}
				else
				{ //�������0
					u8Send0Cnt++;
					if (u8Send0Cnt < 5) 
					{//�����1ms�ĸߵ�ƽ
						//SENDER_WRITE_1;
						WriteHight();
					} 
					else
					{
						if (u8Send0Cnt < 10)  //���4*0.25=1ms�ߵ�ƽ
						{
							//SENDER_WRITE_0;
							WriteLow();
						} 
						else 
						{ //�������0�������
							u8Send0Cnt = 0;
							u8Send0Cnt = 0;
							commandTmp <<= 1;
							index--;
						}
					}
				}
			} 
			else 
			{//������ɣ���ʼ�����Ϣ������
				u8SendEnderCnt++;
				//SENDER_WRITE_0;//����͵�ƽ
				WriteLow();
				if (24 == u8SendEnderCnt)	   //���23*0.25=5.75ms�͵�ƽ�󣬽�������ߵ�ƽ��һ��byte�����������
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
*����:peter
*����:190401
*����:һ�����ݽ��մ���
*����:��
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
if((R_Data==0)&&(Start_Receive_Data==0))  //������͵�ƽ��ʱ��
{
	++u8cnt50ms;
}
if(R_Data==1)          //��ƽΪ��ʱ�жϵ͵�ƽ��ʱ��
{
	if((15<=u8cnt50ms)&&(22>=u8cnt50ms))    //ʱ�䷶Χ�������͵�ƽ�ķ�Χ�ڣ���ʼ����
	{
		Low_Leve_Time=0;
		u8cnt50ms=0;
		High_Leve_Time=0;
		Value=0;
		Total_Data=8;
		Start_Receive_Data=1;
	}
	else if(22<=u8cnt50ms)     //���� ���򲻽���
	{
		Low_Leve_Time=0;
		u8cnt50ms=0;
		High_Leve_Time=0;
		Value=0;
		Total_Data=8;
		Start_Receive_Data=0;
	}
	else                       //�����������
	{
		u8cnt50ms=0;
	}	    
}

if(Start_Receive_Data)   //��ʼ��������
{
	if(Total_Data>0)    //���հ˴�
	{ 
		if(R_Data==1)       //�ߵ�ƽ��ʱ��
		{
			Flag=1;            //�͵�ƽʱ�жϱ�־
			High_Leve_Time++;   //�ߵ�ƽʱ�����
		}
		if((R_Data==0)&&(Flag==1))  //�͵�ƽʱ�жϸߵ�ƽʱ�䣬flag��־λֻ����һ�Σ�																	//ֻ�����´η�ת��ʱ����ܽ����ж�
		{ 
			Value=Value<<1;             //�����������ƣ���λ����ֻ����λ�ߴ�
			Flag=0;                    //���־
			Low_Leve_Time++;            //�͵�ƽʱ�����
			if((High_Leve_Time>=7)&&(High_Leve_Time<=10))  //�ж���Ϊ1
			{
				Low_Leve_Time=0;
				High_Leve_Time=0;
				Value |= 0x01;
				Total_Data--;
			}
			else if((High_Leve_Time>=1)&&(High_Leve_Time<=6))  //�ж���Ϊ0
			{
				Low_Leve_Time=0;
				High_Leve_Time=0;
				Value &= (~0x01);
				Total_Data--;
		       }
		} 
		if((High_Leve_Time>16)||(Low_Leve_Time>8))      //�ݴ����ڽ��������ݺ�����
		{
			Low_Leve_Time=0;
			High_Leve_Time=0;
			u8cnt50ms=0;
			Start_Receive_Data=0;
			Value=0;
			Total_Data=8;
		}
	}
else   //������ɣ�����������
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






