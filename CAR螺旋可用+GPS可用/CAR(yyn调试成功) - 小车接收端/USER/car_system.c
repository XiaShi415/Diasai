#include "config.h"

u32 Car_StatusCounter = 0;

void Delay_Us(u16 utim)
{
		u16 i,j;
	for(i=0;i<utim;i++)
		for(j=0;j<10;j++)
	{
		;
	}
}

void Car_sysTickInit(void)
{
   SysTick_Config(SystemCoreClock/1000);   // 1ms  
}

/*******************************************************************************
  * @��������	SysTick_Handler
  * @����˵��   SysTick�жϷ������жϴ�����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void SysTick_Handler(void)
{
     Car_StatusCounter++;
}

