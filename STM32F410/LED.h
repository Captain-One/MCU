#ifndef __LED_H
#define __LED_H

#include<stm32f4xx.h>

#define LED_RED_PORT						GPIOA
#define LED_RED_PIN							GPIO_Pin_5
#define LED_RED_CLK							RCC_AHB1Periph_GPIOA

#define IO_TO_HI(p,i)						{p->BSRRL=i;}
#define IO_TO_LOW(p,i)					{p->BSRRH=i;}
#define IO_TO_TOG(p,i)					{p->ODR^=i;}

#define LED_RED_ON	            IO_TO_HI(LED_RED_PORT,LED_RED_PIN)						 
#define LED_RED_OFF             IO_TO_LOW(LED_RED_PORT,LED_RED_PIN)
#define LED_RED_TOG             IO_TO_TOG(LED_RED_PORT,LED_RED_PIN) 

void LED_GPIO_Config(void);


#endif
