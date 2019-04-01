#include "system.h"





void main(void)
{
	System_Init();
	while(1)
	{
		System_Handle();
	}
}


