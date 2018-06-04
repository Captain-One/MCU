#ifndef __KEY_H
#define __KEY_H

#include<stm32f4xx.h>

#define KEY1_PORT						    GPIOC
#define KEY1_PIN							  GPIO_Pin_13
#define KEY1_CLK							  RCC_AHB1Periph_GPIOC

#define KEY_ON	            		0						 
#define KEY_OFF             		1

void KEY_GPIO_Init(void);
uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);

#endif
