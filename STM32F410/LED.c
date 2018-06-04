#include "LED.h"

void LED_GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(LED_RED_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin =LED_RED_PIN; 
	GPIO_Init(LED_RED_PORT,&GPIO_InitStructure);
	LED_RED_OFF;
}
