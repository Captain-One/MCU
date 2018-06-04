#ifndef __RCC_H
#define __RCC_H

#include<stm32f4xx.h>

#define PLL_USER_M				 16
#define PLL_USER_N				 400
#define PLL_USER_P				 4
#define PLL_USER_Q				 7
#define PLL_USER_R				 16

#define MCO1_PORT					 GPIOA
#define MCO1_PIN					 GPIO_Pin_8
#define MCO1_CLK 	         RCC_AHB1Periph_GPIOA

#define MCO2_PORT					 GPIOC
#define MCO2_PIN				   GPIO_Pin_9
#define MCO2_CLK 	         RCC_AHB1Periph_GPIOC

void HSE_USER_SetSysClock(uint32_t m,uint32_t n,uint32_t p,uint32_t q,uint32_t r);
void HSI_USER_SetSysClock(uint32_t m,uint32_t n,uint32_t p,uint32_t q,uint32_t r);
void Delay(__IO uint32_t n);
void MCO1_CPIO_Config(void);
void MCO2_CPIO_Config(void);

#endif

