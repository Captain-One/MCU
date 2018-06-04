#include "KEY.h"

void KEY_GPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(KEY1_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed =GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin =KEY1_PIN; 
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);
}

uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
	{
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON);
		return KEY_ON;
	}
	else
	{
		return KEY_OFF;
	}
}
