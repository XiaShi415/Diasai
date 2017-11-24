#include "car_hcsr04.h"
 
u16 msHcCount = 0;//ms����

/*************************************************************************
* ��    �ƣ�void Car_Hcsr04Init(void)
* ��    �ܣ�����ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    �����õ�TIM4������ܽ�ΪPB5��PB6
* ���÷����� 
****************************************************************************/
 
void Car_Hcsr04Init()
{
	
				TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //�������ڶ�ʱ�����õĽṹ��
				NVIC_InitTypeDef NVIC_InitStructure; 
				GPIO_InitTypeDef GPIO_InitStructure;
					
				RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
				//IO��ʼ��
				GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;      					  //���͵�ƽ����
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				  //�������
				GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
				GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
				 
				GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;              //���ص�ƽ����
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;							//��������
				GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);        
	
        //��ʱ����ʼ�� ʹ�û�����ʱ��TIM4       
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   			//ʹ�ܶ�ӦRCCʱ��
        //���ö�ʱ�������ṹ��
        TIM_TimeBaseStructure.TIM_Period = 1000;										 //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ         ������1000Ϊ1ms
        TIM_TimeBaseStructure.TIM_Prescaler =(72-1);								 //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  1M�ļ���Ƶ�� 1US����
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 							//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ  
				
				TIM_ClearFlag(TIM4, TIM_FLAG_Update);  			 //��������жϣ����һ���ж����������ж�
        TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);   	 //�򿪶�ʱ�������ж�
				
				NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
				      
}

/*************************************************************************
* ��    �ƣ�static void OpenTimerForHc() 
* ��    �ܣ�����ʱ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷����� 
****************************************************************************/
static void OpenTimerForHc()       //�򿪶�ʱ��
{
        TIM_SetCounter(TIM4,0);		//�������
        msHcCount = 0;
        TIM_Cmd(TIM4, ENABLE); 		//ʹ��TIMx����
}
 
/*************************************************************************
* ��    �ƣ�static void CloseTimerForHc() 
* ��    �ܣ��رն�ʱ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷����� 
****************************************************************************/
static void CloseTimerForHc()        //�رն�ʱ��
{
        TIM_Cmd(TIM4, DISABLE);  //ʹ��TIMx����
}
 
 
//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)   //TIM4�ж�
{
        if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM6�����жϷ������
        {
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
                msHcCount++;
        }
}
 
/*************************************************************************
* ��    �ƣ�u32 GetEchoTimer(void)
* ��    �ܣ���ȡ��ʱʱ��ֵ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷����� 
****************************************************************************/
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//�õ�MS
        t += TIM_GetCounter(TIM4);//�õ�US
        return t;
}
 


//һ�λ�ȡ������������� ���β��֮����Ҫ���һ��ʱ�䣬���ϻ����ź�
/***************************************************************************
* ��    �ƣ�unsigned int Hcsr04GetLength()
* ��    �ܣ���ȡ����ֵ
* ��ڲ�������
* ���ڲ��������루cm��
* ˵    ����
* ���÷����� 
****************************************************************************/
u32 Hcsr04GetLength()
{
    u32 t = 0;
    float lengthTemp;
    TRIG_Send = 1;      //���Ϳڸߵ�ƽ���
	
    Delay_Us(10);
	
    TRIG_Send = 0;
    while(ECHO_Reci == 0);     //�ȴ����տڸߵ�ƽ���
		OpenTimerForHc();        //�򿪶�ʱ��
		while(ECHO_Reci == 1);
		CloseTimerForHc();        //�رն�ʱ��
		t = GetEchoTimer();        //��ȡʱ��,�ֱ���Ϊ1US
		lengthTemp = ((float)t/58);//cm
    return (u32)lengthTemp; 
}
