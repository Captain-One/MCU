#ifndef __ADVANCE_TIMER_H
#define __ADVANCE_TIMER_H

#include<stm32f4xx.h>

#define ADVANCE_TIM                   			TIM1
#define ADVANCE_TIM_CLK               			RCC_APB2Periph_TIM1

#define ChannelPulse												250

#define ADVANCE_TIM_OCPWM_PORT							GPIOA
#define ADVANCE_TIM_OCPWM_PIN								GPIO_Pin_8
#define ADVANCE_TIM_OCPWM_PIN_CLK						RCC_AHB1Periph_GPIOA
#define ADVANCE_TIM_OCPWM_PINSOURCE  			  GPIO_PinSource8
#define ADVANCE_TIM_OCPWM_AF						    GPIO_AF_TIM1

#define ADVANCE_TIM_OCNPWM_PORT							GPIOA
#define ADVANCE_TIM_OCNPWM_PIN							GPIO_Pin_7
#define ADVANCE_TIM_OCNPWM_PIN_CLK					RCC_AHB1Periph_GPIOA
#define ADVANCE_TIM_OCNPWM_PINSOURCE        GPIO_PinSource7
#define ADVANCE_TIM_OCNPWM_AF               GPIO_AF_TIM1

#define ADVANCE_TIM_BKIN_PORT								GPIOA
#define ADVANCE_TIM_BKIN_PIN								GPIO_Pin_7
#define ADVANCE_TIM_BKIN_PIN_CLK						RCC_AHB1Periph_GPIOA
#define ADVANCE_TIM_BKIN_PINSOURCE       	  GPIO_PinSource7
#define ADVANCE_TIM_BKIN_AF          		    GPIO_AF_TIM1

#define ADVANCE_TIM_IRQn              			TIM1_DAC_IRQn
#define ADVANCE_TIM_IRQHandler							TIM1_DAC_IRQHandler

static void TIMx_GPIO_Config(void);
static void TIMx_Mode_Config(void);
void ADVANCE_TIM_Init(void);

#endif
