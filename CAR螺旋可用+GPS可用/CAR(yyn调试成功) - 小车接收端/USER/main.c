#include "config.h"
#include "vlc.h"
void TimInit()
{
		/*********************************************************/
		/******                定时器2配置           		   *******/
		/*********************************************************/

    NVIC_InitTypeDef NVIC_InitStructure; 
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=1000;		 								/* 自动重装载寄存器周期的值(计数值) */   
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);																		/* 开启时钟 */ 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,DISABLE);		/*先关闭等待使用*/    
}
int main()
{
	//初始化;
  Car_SpeedInit();
	Car_MotorInit();
	Car_CommuncateInit();
	Car_Hcsr04Init();	
  Car_sysTickInit();
	vlc_init(115200);
//	uart_init(9600);	 //GPS串口初始化为9600
	TimInit();
		Car_Go(0,500,Forward);
	while(1)
	{
		//时间片分配 
		if(Car_StatusCounter < 10)
		{
			
		}
		else if(Car_StatusCounter >= 10 && Car_StatusCounter < 20)
		{
			
		}
		else if(Car_StatusCounter >= 20 && Car_StatusCounter < 30)
		{
//			parseGpsBuffer();
//			printGpsBuffer();
		}
		else
		{
			Car_CMDProcess();					//对命令进行处理
			Car_StatusCounter = 0;	 	//时间片计数清零

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);	
	
			
		}			
	}

}



