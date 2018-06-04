/**
  ******************************************************************************
  * @file    LED.h
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

#ifndef __KEY_H
#define __KEY_H

#include <stm8s.h>

#define ON                       1
#define OFF                      0

#define YES                      1
#define NO                       0

#define HIGH                     1
#define LOW                      0
#define KEY_READ_TIME_OUT        50000
#define TIME_OUT_10M             20000

#define STANDARD_MODEL           1
#define AUTO_MODEL               2
#define STANDARD_MULTI_MODEL     3
#define AUTO_MULTI_MODEL         4

#define KEY_PORT                 GPIOB
#define KEY_PIN                  GPIO_PIN_5

#define KEY_TIMER                TIM2

#define KEY_ADD                  0x01
#define KEY_DEC                  0x02
#define KEY_AUTO                 0x04
#define KEY_POWER                0x08

#define KEY_UNLOCK               0x03       /* bit7 and bit5 */
    
#define KEY_FLASH_PORT           GPIOC
#define KEY_FLASH_PIN            GPIO_PIN_1

#define KEY_TOUCH_PORT           GPIOC
#define KEY_TOUCH_PIN            GPIO_PIN_6

#define KEY_START_BIT_THRL       2970
#define KEY_START_BIT_THRH       3810

#define KEY_H_BIT_THRL           1475
#define KEY_H_BIT_THRH           1550

#define KEY_L_BIT_THRL           720
#define KEY_L_BIT_THRH           780
           
void Key_Init_Fun(void);
void Key_Timer_Init_Fun(void);
void Key_Read(void);
void Key_Analysis(void);
void Key_Process_Fun(void);
void Key_Power_Off_Proc(void);
void Key_Level_Up_Proc(void);
void Key_Level_Down_Proc(void);
void Key_Unlock_Proc(void);
void Function_Processe(void);
void Key_Power_On_Proc(void);
void Key_Mute_Proc(void);
void Multi_Auto_Modle_Flash(void);
void Standard_Model_Flash(void);
void Auto_Model_Flash(void);
void Multi_Standard_Modle_Flash(void);

#endif
