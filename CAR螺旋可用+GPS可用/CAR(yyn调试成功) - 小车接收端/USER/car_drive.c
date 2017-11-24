#include "car_drive.h"
#include "stm32f10x.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "misc.h"
unsigned int SelectTim=0;
unsigned int SelectDir =0;
/****************************************************************************
* 名    称：void Motor_PinConfig()
* 功    能：电机驱动管脚配置
* 入口参数：无
* 出口参数：无
* 说    明：用于Car_MotorInit()调用初始化小车驱动控制
* 调用方法： 
****************************************************************************/
void Motor_PinConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB ,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =Car_DIR_L | Car_DIR_R;			//配置方向控制管脚为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(Car_Port_DIR, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin =Car_PWM_L | Car_PWM_R;			//配置PWM控制管脚为复用推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		  	  
  GPIO_Init(Car_Port_PWM, &GPIO_InitStructure);
}

/****************************************************************************
* 名    称：void Motor_PWMConfig()
* 功    能：电机驱动PWM功能配置
* 入口参数：无
* 出口参数：无
* 说    明：用于Car_MotorInit()调用初始化小车驱动控制，用到定时器TIM3的PWM，
*						输出管脚为PA6，PA7
* 调用方法： 
****************************************************************************/
void Motor_PWMConfig()
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_OCInitTypeDef   TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	 /* Time3 base configuration */		 
	  TIM_DeInit(Car_Drive_TIM); 
		TIM_TimeBaseStructure.TIM_Period = 999;      													 //当定时器从0计数到999，即为1000次，为一个定时周期
		TIM_TimeBaseStructure.TIM_Prescaler = 0;	   												 	//设置预分频：不预分频，即为36MHz
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;													//设置时钟分频系数：不分频
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  					//向上计数模式
    TIM_TimeBaseInit(Car_Drive_TIM, &TIM_TimeBaseStructure); // Time base configuration  
		
		/* PWM1 Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   									//配置为PWM模式1
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	 	  			//设置跳变值，当计数器计数到这个值时，电平发生跳变

		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;					//使能通道1
		TIM_OCInitStructure.TIM_Pulse = 500;	 											  	
		TIM_OC1Init(Car_Drive_TIM, &TIM_OCInitStructure);	 										
		TIM_OC1PreloadConfig(Car_Drive_TIM, TIM_OCPreload_Enable);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 500;	 														 	//设置通道2的电平跳变值，输出另外一个占空比的PWM
		TIM_OC2Init(Car_Drive_TIM, &TIM_OCInitStructure);	 													 //使能通道2
		TIM_OC2PreloadConfig(Car_Drive_TIM, TIM_OCPreload_Enable);

		TIM_ARRPreloadConfig(Car_Drive_TIM, ENABLE);																	 // 使能TIM3重载寄存器ARR

		/* TIM3 enable counter */
		TIM_Cmd(Car_Drive_TIM, ENABLE);   
}
/****************************************************************************
* 名    称：void Car_MotorInit()
* 功    能：电机驱动初始化
* 入口参数：无
* 出口参数：无
* 说    明：初始化小车驱动控制，用到定时器TIM3的PWM，输出管脚为PA6，PA7
* 调用方法： 
****************************************************************************/
void Car_MotorInit()
{
	Motor_PinConfig();
	Motor_PWMConfig();
	Car_Go(0,0,Forward);			//小车初始状态静止
	Car_Go(0,500,Back);
}

/****************************************************************************
* 名    称：Car_Go(int iSpeed,int iDir,enum CAR_DIR	eDIR)
* 功    能：小车行进控制
* 入口参数：int iSpeed, int iDir, enum CAR_DIR	eDIR
* 出口参数：无
* 说    明：控制小车前进/后退
*						int iSpeed: 确定小车速度，其取值范围为0~1000，数值越大，速度越快
*           int iDir  : 控制小车转向：  iDir=500时，小车直线向前（向后）
*																		 	 iDir>500时，小车右转向前（向后），且iDir越大，转向越大	
*																			 iDir<500时，小车左转向前（向后），且iDir越大，转向越大	
*						int eDIR  : 控制小车前进或后退  eDIRR = Forward  小车向前
*																					 eDIRR = Back     小车车后				
* 调用方法： Car_Go(500,50,Back);
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
	
	if(pulse_l >= 950)					//小车输出限幅，避免输入参数不在合适范围而到导致的占空比异常
		pulse_l = 950;
	else if(pulse_l <= 0)
		pulse_l = 0;
	
	if(pulse_r >= 950)
		pulse_r = 950;
	else if(pulse_r <= 0)
		pulse_r = 0;	

	Car_Drive_TIM->CCR1 = pulse_l;		//将计算出的占空比数值写入寄存器
	Car_Drive_TIM->CCR2 = pulse_r;		
}

/****************************************************************************
* 名    称：void Car_TurnRound(int iSpeed,enum CAR_DIR	eDIR)
* 功    能：小车原地调头
* 入口参数：int iSpeed, enum CAR_DIR	eDIR
* 出口参数：无
* 说    明：控制小车原地调头
*						int iSpeed: 确定小车速度，其取值范围为0~1000，数值越大，速度越快
*           eDIR      : 控制小车转向方向  Left： 左转调头
*                                         Right: 右转调头
* 调用方法： Car_TurnRound(200,Left)；
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
* 名    称：Car_Go(int iSpeed,int iDir,enum CAR_DIR	eDIR)
* 功    能：小车行进控制
* 入口参数：int iSpeed, int iDir, enum CAR_DIR	eDIR
* 出口参数：无
* 说    明：控制小车前进/后退
*						int iSpeed: 确定小车速度，其取值范围为0~1000，数值越大，速度越快
*           int iDir  : 控制小车转向：  iDir=500时，小车直线向前（向后）
*																		 	 iDir>500时，小车右转向前（向后），且iDir越大，转向越大	
*																			 iDir<500时，小车左转向前（向后），且iDir越大，转向越大	
*						int eDIR  : 控制小车前进或后退  eDIRR = Forward  小车向前
*																					 eDIRR = Back     小车车后				
* 调用方法： Car_Go(500,50,Back);
****************************************************************************/
void Car_SelectSignal()
{
	
	
	GPIO_ResetBits(Car_Port_DIR,Car_DIR_L);
	GPIO_ResetBits(Car_Port_DIR,Car_DIR_R);
	
	Car_Drive_TIM->CCR2 = 1000;		//将计算出的占空比数值写入寄存器
	 if(SelectDir>800)
		 SelectDir = 800;
	Car_Drive_TIM->CCR1 = SelectDir;		
}