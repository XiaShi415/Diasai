#include  "stm32f10x.h"
#include  "time.h"
#include "stm32f10x_it.h"
#include "misc.h"

void frequencyInit(void)
{
  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef   TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 |  RCC_APB1Periph_TIM4 |  RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

/*********************************************************/
/******               ��ʱ��2����        		   *******/
/*********************************************************/

    TIM_TimeBaseStructure.TIM_Period=10000-1;	   //1ms 
    TIM_TimeBaseStructure.TIM_Prescaler= 72-1;	
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2, ENABLE);

/*********************************************************/
/******               ��ʱ��4����        		   *******/
/*********************************************************/
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

 
   TIM_DeInit(TIM4);
   TIM_TimeBaseStructure.TIM_Period =65535; 
   TIM_TimeBaseStructure.TIM_Prescaler = 0; 
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
   TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure);
   TIM_ClearFlag(TIM4, TIM_FLAG_Update);
   TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
   TIM_Cmd(TIM4, ENABLE); 
	

/*********************************************************/
/******               ������3����        		   *******/
/*********************************************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_DeInit(TIM3); 
    TIM_TimeBaseStructure.TIM_Period = 65535; 
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; /*��ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
	                                                         ʹ�õĲ���Ƶ��֮��ķ�Ƶ��Ϊ1*/
                                                                            
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure); // Time base configuration  
	TIM_TIxExternalClockConfig(TIM3,TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_Rising,0);
    TIM_SetCounter(TIM3, 0);   // ���������CNT	 
    TIM_Cmd(TIM3,ENABLE); 
	
/********************************************************/
/********              ���벶׽����			    *********/
/********************************************************/
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
//    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	  
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   TIM_DeInit(TIM5);
   TIM_TimeBaseStructure.TIM_Period =100-1; 
   TIM_TimeBaseStructure.TIM_Prescaler =71; 
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
   TIM_TimeBaseInit( TIM5, &TIM_TimeBaseStructure); 

   TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;//ͨ��ѡ��     
   TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�½���  
   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//�ܽ���Ĵ�����Ӧ��ϵ    
   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//��Ƶ��    
   TIM_ICInitStructure.TIM_ICFilter = 0x00;         //�˲����ã������������������϶������ȶ�0x0��0xF 
   TIM_ICInit(TIM5, &TIM_ICInitStructure);         //��ʼ�� 

    TIM_ClearFlag(TIM5, TIM_IT_CC4);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    TIM_ITConfig(TIM5,TIM_IT_CC4,ENABLE);
	TIM_ITConfig(TIM5,TIM_FLAG_Update,ENABLE);
	TIM_Cmd(TIM5, ENABLE);	

}





/*********************************************************************************/		 
/*****************************   �ⲿ�ж����� ************************************/
/*********************************************************************************/	
void EXTI_ENABLE(void)
{ 

//     GPIO_InitTypeDef GPIO_InitStructure;
//	 EXTI_InitTypeDef EXTI_InitStructure;
//     NVIC_InitTypeDef NVIC_InitStructure; 

//	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	  NVIC_Init(&NVIC_InitStructure);
//									
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 //��������
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	  GPIO_Init(GPIOA, &GPIO_InitStructure);
//		/* EXTI line mode config */
//	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
//	  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�ж�
//	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	  EXTI_Init(&EXTI_InitStructure);  

///***********************************************************************/

//}
//void EXTI_DISABLE(void)
//{ 

//	  NVIC_InitTypeDef NVIC_InitStructure; 

//	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
//	  NVIC_Init(&NVIC_InitStructure);


}
void Falling(void)
{
	TIM_ICInitTypeDef   TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//�½���  
    TIM_ICInit(TIM5, &TIM_ICInitStructure);   		
}
void Rising(void)
{
	TIM_ICInitTypeDef   TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�½���  
    TIM_ICInit(TIM5, &TIM_ICInitStructure);   		
}



