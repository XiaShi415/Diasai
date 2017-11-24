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
  * @函数名称	SysTick_Handler
  * @函数说明   SysTick中断发生的中断处理函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void SysTick_Handler(void)
{
     Car_StatusCounter++;
}

