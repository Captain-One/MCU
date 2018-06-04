#ifndef __EXTI_H
#define __EXTI_H

#include<stm32f4xx.h>

#define KEY1_PORT								  GPIOC
#define KEY1_PIN									GPIO_Pin_13
#define KEY1_CLK									RCC_AHB1Periph_GPIOC

#define KEY1_EXTI_PORTSOURCE			EXTI_PortSourceGPIOC
#define KEY1_EXTI_PINSOURCE 			EXTI_PinSource13
#define KEY1_EXTI_LINE						EXTI_Line13
#define KEY1_EXTI_IRQ							EXTI15_10_IRQn

#define KEY1_EXTI_IRQHandler			EXTI15_10_IRQHandler

static void NVIC_Configurationg(void);
void EXTI_Key_Config(void);

#endif
