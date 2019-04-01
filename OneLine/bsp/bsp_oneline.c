#include "bsp_oneline.h"

PROTOCOL_SENDMODE Protocol_SendMode;

PROTOCOL_REVMODE  Protocol_RevMode;


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
/**********************************************************************************
*����:peter
*����:190401
*����:һ�����ݷ��ʹ���
*����:��
***********************************************************************************/
void OneLineOut_Handle(void)
{
	Protocol_SendMode.u16SenderCnts ++;
	if(Protocol_SendMode.u16SenderCnts == SerialDataTime)
	{
		Protocol_SendMode.u16SenderCnts = 0;
		Protocol_SendMode.ProcessFlag = 1;
		Protocol_SendMode.CommandTemp = Protocol_SendMode.Command;
	}
	Data_Handle();
}

/**********************************************************************************
*����:peter
*����:190401
*����:һ�����ݽ��մ���
*����:��
***********************************************************************************/
void OneLineIn_Handle(void)
{
	Protocol_RevMode.Rev_Data = ONELINE_PIN;
	if((!Protocol_RevMode.Rev_Data)&&(!Protocol_RevMode.StartRevDataFlag))
	{
		++ Protocol_RevMode.u8Cnts50ms;
	}
	if(Protocol_RevMode.Rev_Data)
	{
		if((Protocol_RevMode.u8Cnts50ms >= 15)&&(Protocol_RevMode.u8Cnts50ms <= 22))
		{
			Protocol_RevMode.HightLevelTime = 0;
			Protocol_RevMode.LowLevelTime = 0;
			Protocol_RevMode.u8Cnts50ms= 0;
			Protocol_RevMode.Value = 0;
			Protocol_RevMode.StartRevDataFlag = 1;
			Protocol_RevMode.Total_Data = 8;
		}
		else if(Protocol_RevMode.u8Cnts50ms > 22)
		{
			Protocol_RevMode.HightLevelTime = 0;
			Protocol_RevMode.LowLevelTime = 0;
			Protocol_RevMode.u8Cnts50ms= 0;
			Protocol_RevMode.Value = 0;
			Protocol_RevMode.StartRevDataFlag = 0;
			Protocol_RevMode.Total_Data = 8;
		}
		else
		{
			Protocol_RevMode.u8Cnts50ms = 0;
		}
	}
	if(Protocol_RevMode.StartRevDataFlag)
	{
		if(Protocol_RevMode.Total_Data > 0)
		{
			if(Protocol_RevMode.Rev_Data)
			{
				Protocol_RevMode.LowLevelFlag = 1;
				Protocol_RevMode.HightLevelTime ++;
			}
			if((!Protocol_RevMode.Rev_Data)&&(Protocol_RevMode.LowLevelFlag))
			{
				Protocol_RevMode.LowLevelFlag = 0;
				Protocol_RevMode.Value <<= 1;
				Protocol_RevMode.LowLevelTime ++;
				if((Protocol_RevMode.HightLevelTime >= 7)&&(Protocol_RevMode.HightLevelTime <= 10))
				{
					Protocol_RevMode.HightLevelTime = 0;
					Protocol_RevMode.LowLevelTime = 0;
					Protocol_RevMode.Value |= 0x01;
					Protocol_RevMode.Total_Data --;
				}
				if((Protocol_RevMode.HightLevelTime >= 1)&&(Protocol_RevMode.HightLevelTime <= 6))
				{
					Protocol_RevMode.HightLevelTime = 0;
					Protocol_RevMode.LowLevelTime = 0;
					Protocol_RevMode.Value &= ~0x01;
					Protocol_RevMode.Total_Data --;
				}
				if((Protocol_RevMode.HightLevelTime > 16)&&(Protocol_RevMode.LowLevelTime>8))
				{
					Protocol_RevMode.HightLevelTime = 0;
					Protocol_RevMode.LowLevelTime = 0;
					Protocol_RevMode.u8Cnts50ms= 0;
					Protocol_RevMode.Value = 0;
					Protocol_RevMode.StartRevDataFlag = 0;
					Protocol_RevMode.Total_Data = 8;
				}
			}
		}
		else
		{
			Protocol_RevMode.HightLevelTime = 0;
			Protocol_RevMode.LowLevelTime = 0;
			Protocol_RevMode.u8Cnts50ms= 0;
			Protocol_RevMode.Data = Protocol_RevMode.Value;
			Protocol_RevMode.StartRevDataFlag = 0;
			Protocol_RevMode.Total_Data = 8;
		}
	}
	
}






