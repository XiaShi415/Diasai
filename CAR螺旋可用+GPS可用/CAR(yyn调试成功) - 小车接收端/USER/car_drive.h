/************************************************
**                С����������ͷ�ļ�           **
**																						 **
************************************************/
#ifndef _CARDRIVE_H
#define _CARDRIVE_H

#include "config.h"

/*************************************************
**                  �����ܽ� 
***************************************************/
#define Car_PWM_L 				GPIO_Pin_6  		//�����PWM���ƹܽ�
#define Car_PWM_R					GPIO_Pin_7			//�Ҳ���PWM���ƹܽ�

#define Car_DIR_L 				GPIO_Pin_8			//���������ǰ�󣩿��ƹܽ�
#define Car_DIR_R  				GPIO_Pin_7			//�Ҳ�������ǰ�󣩿��ƹܽ�

#define Car_Port_PWM			GPIOA						//PWM����GPIO��
#define Car_Port_DIR			GPIOB						//�������GPIO��
#define Car_Drive_TIM     TIM3						//���ڵ�����ƵĶ�ʱ��

enum CAR_DIR															//����С������ѡ��
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
