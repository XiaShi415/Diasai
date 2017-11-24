#include "config.h"
#include "vlc.h"
void TimInit()
{
		/*********************************************************/
		/******                ��ʱ��2����           		   *******/
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
    TIM_TimeBaseStructure.TIM_Period=1000;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */   
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);																		/* ����ʱ�� */ 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,DISABLE);		/*�ȹرյȴ�ʹ��*/    
}
int main()
{
	//��ʼ��;
  Car_SpeedInit();
	Car_MotorInit();
	Car_CommuncateInit();
	Car_Hcsr04Init();	
  Car_sysTickInit();
	vlc_init(115200);
//	uart_init(9600);	 //GPS���ڳ�ʼ��Ϊ9600
	TimInit();
		Car_Go(0,500,Forward);
	while(1)
	{
		//ʱ��Ƭ���� 
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
			Car_CMDProcess();					//��������д���
			Car_StatusCounter = 0;	 	//ʱ��Ƭ��������

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);	
	
			
		}			
	}

}



