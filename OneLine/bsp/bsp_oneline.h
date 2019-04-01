#ifndef _BSP_ONELINE_H_
#define _BSP_ONELINE_H_

#include "sc92f732x_c.h"


#define ONELINE_PORT     P0
#define ONELINE_PIN       P00

#define WriteHight()      ONELINE_PIN=1
#define WriteLow()        ONELINE_PIN=0

#define SerialDataTime     200
#define HeadDataTime      20
#define EndDataTime        24

#define Data1Time9Cnts    9
#define Data1Time10Cnts  10
#define Data0Time5Cnts    5
#define Data0Time10Cnts  10

#if 0

typedef struct
{
	unsigned int u16SenderCnts;
	unsigned int u16SendHanderCnts;
	unsigned int u16SendEnderCnts;
	unsigned int u16Send0Cnts;
	unsigned int u16Send1Cnts;
	unsigned char Command;
	unsigned char CommandTemp;
	unsigned char ProcessFlag;
	unsigned char index;
}
PROTOCOL_SENDMODE;

#endif


void OneLineIn_Init(void);
void OneLineOut_Init(void);

void OneLineOut_Handle(void);
void OneLineIn_Handle(void);





#endif

