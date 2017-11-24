#include "Car_Speed.h"

unsigned int Car_SpeedPulse_L = 0;
unsigned int Car_SpeedPulse_R = 0;


/****************************************************************************
* 名    称：void Car_SpeedInit(void)
* 功    能：测速初始化
* 入口参数：无
* 出口参数：无
* 说    明，用到EXIT0，EXIT1，输出管脚为PB0，PB1
* 调用方法： 
****************************************************************************/
void Car_SpeedInit(void)
{ 

	  GPIO_InitTypeDef GPIO_InitStructure;
	  EXTI_InitTypeDef EXTI_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure; 

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
									
	  GPIO_InitStructure.GPIO_Pin = Car_SpeedPin_L;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(Car_SpeedPort, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = Car_SpeedPin_R;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(Car_SpeedPort, &GPIO_InitStructure);

	/* EXTI line mode config  B0  L */		   
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //中断  UP
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);  
	/* EXTI line mode config  B1 R */	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			 
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //中断 UP
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);  
}
/****************************************************************************
* 名    称：void Car_GetSpeedValue(void)
* 功    能：获取速度值
* 入口参数：无
* 出口参数：
* 说    明，在主函数循环中调用，返回值为25ms左右轮脉冲数均值，精确数值需根据硬件调试
* 调用方法： 
****************************************************************************/
unsigned int Car_GetSpeedValue(void)
{
		unsigned int tmp;
	
	  tmp = (Car_SpeedPulse_L + Car_SpeedPulse_R)/2; 
	  Car_SpeedPulse_L = 0;
    Car_SpeedPulse_R = 0;
	
	  return tmp;
}

/****************************************************************************
* 名    称：void EXTI0_IRQHandler(void)
* 功    能：EXTI0中断函数
* 入口参数：无
* 出口参数：无
* 说    明：用于记录测速码盘脉冲数
* 调用方法： 
****************************************************************************/
void EXTI0_IRQHandler(void)		//左轮计速
{

    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {	
	
			Car_SpeedPulse_L++;
	    EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/****************************************************************************
* 名    称：void EXTI1_IRQHandler(void)
* 功    能：EXTI1中断函数
* 入口参数：无
* 出口参数：无
* 说    明：用于记录测速码盘脉冲数
* 调用方法： 
****************************************************************************/
void EXTI1_IRQHandler(void)	    //右轮计速
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
			Car_SpeedPulse_R++;
			EXTI_ClearITPendingBit(EXTI_Line1);
		}
}

