/**
  ******************************************************************************
  * @file    delay.c 
  * @author  CTS Application Team
  * @version V0.1.0
  * @date    5/3/2012
  * @brief   This file provides a set of delay functions
  ******************************************************************************
  * @copy
  *
  * 该固件的目的是为用户提供硬件编程接口，节省编程时间。因此，程序作者将不会为此
  * 承担任何直接、间接或相应的损害赔偿方面的任何索赔要求。
  *
  * <h2><center>&copy; COPYRIGHT 2012 HEBUT</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"

/** @addtogroup camposition
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

///**
//  * @brief  ms级延时@100MHz
//  * @param  delay    The delay required
//  * @retval None
//  */
//void delay_ms(uint32_t n_ms)
//{
//    volatile  uint32_t  i;
//
//
//    for (i = 0; i < n_ms; i++) {
//        delay_us(1000);
//    }
//}

/**
  * @brief  ms级延时@72MHz
  * @param  delay    The delay required
  * @retval None
  */
void delay_ms(u32 n_ms)
{
    vu32  i;


    for (i = 0; i < n_ms; i++) {
        delay_us(1000);
    }
}
///**
//  * @brief  us级延时@100MHz
//  * @param  delay    The delay required
//  * @retval None
//  */
//void delay_us(uint32_t n_us)
//{
//    volatile  uint32_t  i;
//
//
//    for (i = 0; i < (4 * n_us); i++) {    /* This logic was tested. It gives app. 1 micro sec delay        */
//        ;
//    }
//}

/**
  * @brief  us级延时@72MHz
  * @param  delay    The delay required
  * @retval None
  */
void delay_us(u32 n_us)
{
    vu32  i;


    for (i = 0; i < (n_us); i++) {    /* This logic was tested. It gives app. 1 micro sec delay        */
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
        __nop();//__nop();
//        __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    }
}
/*********************** (C) COPYRIGHT 2012 HEBUT **************END OF FILE****/
