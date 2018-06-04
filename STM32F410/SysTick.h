#ifndef __SYSTICK_H
#define __SYSTICK_H

#include<stm32f4xx.h>

extern uint32_t TimeDelya;

void SysTick_Init(void);
void Delay_SysTick_us(uint32_t n);

#endif
