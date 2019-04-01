#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#include "sc92f732x_c.h"


#define KEY_PORT      P2
#define KEY_PIN        P21










void Key_Init(void);
unsigned char Read_Key(unsigned int GPIO_PORT,unsigned char GPIO_PIN);



#endif



