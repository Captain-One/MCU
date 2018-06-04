#ifndef __BASIC_TIMER_H
#define __BASIC_TIMER_H

#include<stm32f4xx.h>

#define BASIC_TIM                   TIM6
#define BASIC_TIM_CLK               RCC_APB1Periph_TIM6

#define BASIC_TIM_IRQn              TIM6_DAC_IRQn
#define BASIC_TIM_IRQHandler				TIM6_DAC_IRQHandler

void BASIC_TIM_Init(void);
static void TIMx_Mode_Config(void);
static void TIMx_NVIC_Configurationg(void);


#endif
