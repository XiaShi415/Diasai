#ifndef _CAR_HCSR04_H_
#define _CAR_HCSR04_H_

#include "config.h"

#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_5
#define HCSR04_ECHO     GPIO_Pin_6

#define TRIG_Send  PBout(5)
#define ECHO_Reci  PBin(6)

u32 Hcsr04GetLength(void);
void Car_Hcsr04Init(void);

#endif

