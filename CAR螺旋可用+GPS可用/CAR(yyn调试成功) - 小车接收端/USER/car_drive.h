/************************************************
**                小车驱动配置头文件           **
**																						 **
************************************************/
#ifndef _CARDRIVE_H
#define _CARDRIVE_H

#include "config.h"

/*************************************************
**                  驱动管脚 
***************************************************/
#define Car_PWM_L 				GPIO_Pin_6  		//左侧电机PWM控制管脚
#define Car_PWM_R					GPIO_Pin_7			//右侧电机PWM控制管脚

#define Car_DIR_L 				GPIO_Pin_8			//左侧电机方向（前后）控制管脚
#define Car_DIR_R  				GPIO_Pin_7			//右侧电机方向（前后）控制管脚

#define Car_Port_PWM			GPIOA						//PWM控制GPIO口
#define Car_Port_DIR			GPIOB						//方向控制GPIO口
#define Car_Drive_TIM     TIM3						//用于电机控制的定时器

enum CAR_DIR															//用于小车方向选择
		{	
			Forward,
			Back,
			Left,
			Right
		};							

		
void Car_MotorInit(void);
void Car_Go(int iSpeed,int iDir,enum CAR_DIR	eDIR);
void Car_TurnRound(int iSpeed,enum CAR_DIR	eDIR);
void Car_SelectSignal(); 
		
extern unsigned int SelectTim;
extern unsigned int SelectDir;
		
#endif
