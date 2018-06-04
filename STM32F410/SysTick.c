#include "SysTick.h"

void SysTick_Init(void)
{
	//SystemCoreClock/1000000   1us
	//SystemCoreClock/100000    10us
	//SystemCoreClock/1000      1ms     
	if(SysTick_Config(SystemCoreClock/1000000))
	{
		while(1);
	}
}
void Delay_SysTick_us(uint32_t n)
{
	TimeDelya=n;
	while(TimeDelya!=0);
}
