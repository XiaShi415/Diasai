#ifndef _CAR_SPEED_H_
#define _CAR_SPEED_H_

#include "config.h"


#define Car_SpeedPin_L GPIO_Pin_0  //���ٹܽ�
#define Car_SpeedPin_R GPIO_Pin_1  
#define Car_SpeedPort  GPIOB		

extern unsigned int Car_SpeedPulse_L;			 //����������
extern unsigned int Car_SpeedPulse_R;			 //����������

void Car_SpeedInit(void);
unsigned int Car_GetSpeedValue(void);

#endif
