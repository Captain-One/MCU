/**
  ******************************************************************************
  * @file    GPIO_Config.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define LED Pin, PORT and Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_CONFIG_H
#define __GPIO_CONFIG_H

#include <stm8s.h>

#define ON                              1
#define OFF                             0

#define FULL_CHARGE_PORT                GPIOC
#define FULL_CHARGE_PIN                 GPIO_PIN_3

#define PROBE_POWER_PORT                GPIOC
#define PROBE_POWER_PIN                 GPIO_PIN_5

#define HV_CONTROL_PORT                 GPIOE
#define HV_CONTROL_PIN                  GPIO_PIN_7

#define MULTI_PLUS_CTR_PORT             GPIOB
#define MULTI_PLUS_CTR_PIN              GPIO_PIN_0

#define SET_IO_HIGH(p,i)                {p->ODR |= i;}
#define SET_IO_LOW(p,i)                 {p->ODR &= ~i;}
#define SET_IO_TOG(p,i)                 {p->ODR ^= i;}

#define SET_IO_DIR_OUT(P,i)             {p->DDR |= i;}
#define SET_IO_DIR_INPUT(P,i)           {p->DDR &= ~i;}
#define SET_IO_DIR_TOG(P,i)             {p->DDR ^= i;}

void HV_Control_IO_Init_Fun(void);
void Init_Fun(void);
void IWDG_Init(void);

#endif
