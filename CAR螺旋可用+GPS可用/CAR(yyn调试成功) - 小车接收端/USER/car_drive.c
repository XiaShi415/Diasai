#include "car_drive.h"
#include "stm32f10x.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "misc.h"
unsigned int SelectTim=0;
unsigned int SelectDir =0;
/****************************************************************************
* ��    �ƣ�void Motor_PinConfig()
* ��    �ܣ���������ܽ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ��������Car_MotorInit()���ó�ʼ��С����������
* ���÷����� 
****************************************************************************/
void Motor_PinConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB ,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =Car_DIR_L | Car_DIR_R;			//���÷�����ƹܽ�Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(Car_Port_DIR, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin =Car_PWM_L | Car_PWM_R;			//����PWM���ƹܽ�Ϊ�����������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  	  
  GPIO_Init(Car_Port_PWM, &GPIO_InitStructure);
}

/****************************************************************************
* ��    �ƣ�void Motor_PWMConfig()
* ��    �ܣ��������PWM��������
* ��ڲ�������
* ���ڲ�������
* ˵    ��������Car_MotorInit()���ó�ʼ��С���������ƣ��õ���ʱ��TIM3��PWM��
*						����ܽ�ΪPA6��PA7
* ���÷����� 
****************************************************************************/
void Motor_PWMConfig()
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_OCInitTypeDef   TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	 /* Time3 base configuration */		 
	  TIM_DeInit(Car_Drive_TIM); 
		TIM_TimeBaseStructure.TIM_Period = 999;      													 //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
		TIM_TimeBaseStructure.TIM_Prescaler = 0;	   												 	//����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;													//����ʱ�ӷ�Ƶϵ��������Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  					//���ϼ���ģʽ
    TIM_TimeBaseInit(Car_Drive_TIM, &TIM_TimeBaseStructure); // Time base configuration  
		
		/* PWM1 Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   									//����ΪPWMģʽ1
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	 	  			//��������ֵ�������������������ֵʱ����ƽ��������

		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;					//ʹ��ͨ��1
		TIM_OCInitStructure.TIM_Pulse = 500;	 											  	
		TIM_OC1Init(Car_Drive_TIM, &TIM_OCInitStructure);	 										
		TIM_OC1PreloadConfig(Car_Drive_TIM, TIM_OCPreload_Enable);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 500;	 														 	//����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
		TIM_OC2Init(Car_Drive_TIM, &TIM_OCInitStructure);	 													 //ʹ��ͨ��2
		TIM_OC2PreloadConfig(Car_Drive_TIM, TIM_OCPreload_Enable);

		TIM_ARRPreloadConfig(Car_Drive_TIM, ENABLE);																	 // ʹ��TIM3���ؼĴ���ARR

		/* TIM3 enable counter */
		TIM_Cmd(Car_Drive_TIM, ENABLE);   
}
/****************************************************************************
* ��    �ƣ�void Car_MotorInit()
* ��    �ܣ����������ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ������ʼ��С���������ƣ��õ���ʱ��TIM3��PWM������ܽ�ΪPA6��PA7
* ���÷����� 
****************************************************************************/
void Car_MotorInit()
{
	Motor_PinConfig();
	Motor_PWMConfig();
	Car_Go(0,0,Forward);			//С����ʼ״̬��ֹ
	Car_Go(0,500,Back);
}

/****************************************************************************
* ��    �ƣ�Car_Go(int iSpeed,int iDir,enum CAR_DIR	eDIR)
* ��    �ܣ�С���н�����
* ��ڲ�����int iSpeed, int iDir, enum CAR_DIR	eDIR
* ���ڲ�������
* ˵    ��������С��ǰ��/����
*						int iSpeed: ȷ��С���ٶȣ���ȡֵ��ΧΪ0~1000����ֵԽ���ٶ�Խ��
*           int iDir  : ����С��ת��  iDir=500ʱ��С��ֱ����ǰ�����
*																		 	 iDir>500ʱ��С����ת��ǰ����󣩣���iDirԽ��ת��Խ��	
*																			 iDir<500ʱ��С����ת��ǰ����󣩣���iDirԽ��ת��Խ��	
*						int eDIR  : ����С��ǰ�������  eDIRR = Forward  С����ǰ
*																					 eDIRR = Back     С������				
* ���÷����� Car_Go(500,50,Back);
****************************************************************************/
void Car_Go(int iSpeed,int iDir,enum CAR_DIR	eDIR)
{
	int pulse_l = 0;
	int pulse_r = 0;
	
	if(eDIR == Forward)
	{
		GPIO_ResetBits(Car_Port_DIR,Car_DIR_L);
		GPIO_ResetBits(Car_Port_DIR,Car_DIR_R);
	}
	else if(eDIR == Back)
	{
		
		GPIO_SetBits(Car_Port_DIR,Car_DIR_L);
		GPIO_SetBits(Car_Port_DIR,Car_DIR_R);	
		
		iSpeed = 1000 - iSpeed;
	}

	iDir = iDir - 500;
	pulse_l = iSpeed + iDir;
	pulse_r = iSpeed - iDir;
	
	if(pulse_l >= 950)					//С������޷�����������������ں��ʷ�Χ�������µ�ռ�ձ��쳣
		pulse_l = 950;
	else if(pulse_l <= 0)
		pulse_l = 0;
	
	if(pulse_r >= 950)
		pulse_r = 950;
	else if(pulse_r <= 0)
		pulse_r = 0;	

	Car_Drive_TIM->CCR1 = pulse_l;		//���������ռ�ձ���ֵд��Ĵ���
	Car_Drive_TIM->CCR2 = pulse_r;		
}

/****************************************************************************
* ��    �ƣ�void Car_TurnRound(int iSpeed,enum CAR_DIR	eDIR)
* ��    �ܣ�С��ԭ�ص�ͷ
* ��ڲ�����int iSpeed, enum CAR_DIR	eDIR
* ���ڲ�������
* ˵    ��������С��ԭ�ص�ͷ
*						int iSpeed: ȷ��С���ٶȣ���ȡֵ��ΧΪ0~1000����ֵԽ���ٶ�Խ��
*           eDIR      : ����С��ת����  Left�� ��ת��ͷ
*                                         Right: ��ת��ͷ
* ���÷����� Car_TurnRound(200,Left)��
****************************************************************************/
void Car_TurnRound(int iSpeed,enum CAR_DIR	eDIR)
{
	int pulse_l = 0;
	int pulse_r = 0;
	
	pulse_l = iSpeed;
	pulse_r = iSpeed;
	
	if(pulse_l >= 950)			//??????,??????????????????????
		pulse_l = 950;
	else if(pulse_l <= 20)
		pulse_l = 20;
	
	if(pulse_r >= 950)
		pulse_r = 950;
	else if(pulse_r <= 20)
		pulse_r = 20;	
	
	/**   ????????????  **/
	if(eDIR ==Right )
	{
		GPIO_ResetBits(Car_Port_DIR,Car_DIR_L);
		GPIO_SetBits(Car_Port_DIR,Car_DIR_R);
		
		Car_Drive_TIM->CCR1 = pulse_l;		
		Car_Drive_TIM->CCR2 = 1000 - pulse_r;		
	}
	else if(eDIR == Left)
	{
		GPIO_SetBits(Car_Port_DIR,Car_DIR_L);
		GPIO_ResetBits(Car_Port_DIR,Car_DIR_R);
		
		Car_Drive_TIM->CCR1 = 1000 - pulse_l;		
		Car_Drive_TIM->CCR2 = pulse_r;		
	}
}
/****************************************************************************
* ��    �ƣ�Car_Go(int iSpeed,int iDir,enum CAR_DIR	eDIR)
* ��    �ܣ�С���н�����
* ��ڲ�����int iSpeed, int iDir, enum CAR_DIR	eDIR
* ���ڲ�������
* ˵    ��������С��ǰ��/����
*						int iSpeed: ȷ��С���ٶȣ���ȡֵ��ΧΪ0~1000����ֵԽ���ٶ�Խ��
*           int iDir  : ����С��ת��  iDir=500ʱ��С��ֱ����ǰ�����
*																		 	 iDir>500ʱ��С����ת��ǰ����󣩣���iDirԽ��ת��Խ��	
*																			 iDir<500ʱ��С����ת��ǰ����󣩣���iDirԽ��ת��Խ��	
*						int eDIR  : ����С��ǰ�������  eDIRR = Forward  С����ǰ
*																					 eDIRR = Back     С������				
* ���÷����� Car_Go(500,50,Back);
****************************************************************************/
void Car_SelectSignal()
{
	
	
	GPIO_ResetBits(Car_Port_DIR,Car_DIR_L);
	GPIO_ResetBits(Car_Port_DIR,Car_DIR_R);
	
	Car_Drive_TIM->CCR2 = 1000;		//���������ռ�ձ���ֵд��Ĵ���
	 if(SelectDir>800)
		 SelectDir = 800;
	Car_Drive_TIM->CCR1 = SelectDir;		
}