#include "Car_Speed.h"

unsigned int Car_SpeedPulse_L = 0;
unsigned int Car_SpeedPulse_R = 0;


/****************************************************************************
* ��    �ƣ�void Car_SpeedInit(void)
* ��    �ܣ����ٳ�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    �����õ�EXIT0��EXIT1������ܽ�ΪPB0��PB1
* ���÷����� 
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
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�ж�  UP
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);  
	/* EXTI line mode config  B1 R */	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			 
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�ж� UP
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);  
}
/****************************************************************************
* ��    �ƣ�void Car_GetSpeedValue(void)
* ��    �ܣ���ȡ�ٶ�ֵ
* ��ڲ�������
* ���ڲ�����
* ˵    ������������ѭ���е��ã�����ֵΪ25ms��������������ֵ����ȷ��ֵ�����Ӳ������
* ���÷����� 
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
* ��    �ƣ�void EXTI0_IRQHandler(void)
* ��    �ܣ�EXTI0�жϺ���
* ��ڲ�������
* ���ڲ�������
* ˵    �������ڼ�¼��������������
* ���÷����� 
****************************************************************************/
void EXTI0_IRQHandler(void)		//���ּ���
{

    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {	
	
			Car_SpeedPulse_L++;
	    EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/****************************************************************************
* ��    �ƣ�void EXTI1_IRQHandler(void)
* ��    �ܣ�EXTI1�жϺ���
* ��ڲ�������
* ���ڲ�������
* ˵    �������ڼ�¼��������������
* ���÷����� 
****************************************************************************/
void EXTI1_IRQHandler(void)	    //���ּ���
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
			Car_SpeedPulse_R++;
			EXTI_ClearITPendingBit(EXTI_Line1);
		}
}

