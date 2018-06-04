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

#include "GPIO_Config.h"
#include "delay.h"
#include "clock_config.h"
#include "HV_Charge.h"
#include "stm8s_gpio.h"
#include "LED.h"
#include "UART.h"
#include "KEY_FTC332.h"
#include "ADC.h"
#include "EEPROM.h"

void HV_Control_IO_Init_Fun(void)
{
  HV_CONTROL_PORT->ODR |= HV_CONTROL_PIN;
  HV_CONTROL_PORT->DDR |= HV_CONTROL_PIN;
  HV_CONTROL_PORT->CR1 |= HV_CONTROL_PIN;
  HV_CONTROL_PORT->CR2 &= ~HV_CONTROL_PIN;
  
  MULTI_PLUS_CTR_PORT->ODR |= MULTI_PLUS_CTR_PIN;
  MULTI_PLUS_CTR_PORT->DDR |= MULTI_PLUS_CTR_PIN;
  MULTI_PLUS_CTR_PORT->CR1 |= MULTI_PLUS_CTR_PIN;
  MULTI_PLUS_CTR_PORT->CR2 |= MULTI_PLUS_CTR_PIN;  /* Fast Speed */
  
  PROBE_POWER_PORT->ODR &= ~(PROBE_POWER_PIN + FULL_CHARGE_PIN);
  PROBE_POWER_PORT->DDR |= PROBE_POWER_PIN + FULL_CHARGE_PIN;
  PROBE_POWER_PORT->CR1 |= PROBE_POWER_PIN + FULL_CHARGE_PIN;
  PROBE_POWER_PORT->CR2 &= ~(PROBE_POWER_PIN + FULL_CHARGE_PIN);
}

void IWDG_Init(void)
{
  IWDG->KR = 0xCC; //启动IWDG
  IWDG->KR = 0x55; //解除 PR 及 RLR 的写保护
  IWDG->RLR = 0xff; //看门狗计数器重装载数值       
  IWDG->PR = 0x06; //分频系数为256，最长超时时间为：1.02S
  IWDG->KR = 0xAA; //刷新IDDG，避免产生看门狗复位，同时恢复 PR 及 RLR 的写保护状态
}

void Init_Fun(void)
{
  Clock_Config();
  HV_Control_IO_Init_Fun();
  GPIO_Init(FLASH_TRIG_PORT,FLASH_TRIG_PIN,GPIO_MODE_OUT_OD_HIZ_FAST);
  LED_GPIO_Init();
  DEBUG_USART_Config();
  HV_PID_Init();
  Key_Init_Fun();
  ADC_Init_Fun();
  EEPROM_IO_Init();
  GPIO_Init(BEEP_PORT,BEEP_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  BEEP_ON(BEEP_PORT,BEEP_PIN);
  Delay_ms(300);
  BEEP_OFF(BEEP_PORT,BEEP_PIN); 
  IWDG_Init();
}
