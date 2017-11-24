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
  * �ù̼���Ŀ����Ϊ�û��ṩӲ����̽ӿڣ���ʡ���ʱ�䡣��ˣ��������߽�����Ϊ��
  * �е��κ�ֱ�ӡ���ӻ���Ӧ�����⳥������κ�����Ҫ��
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
//  * @brief  ms����ʱ@100MHz
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
  * @brief  ms����ʱ@72MHz
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
//  * @brief  us����ʱ@100MHz
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
  * @brief  us����ʱ@72MHz
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
