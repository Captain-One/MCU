#include "EXTI.h"

static void NVIC_Configurationg(void)
{
	NVIC_InitTypeDef NVIC_Init_Structure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_Init_Structure.NVIC_IRQChannel = KEY1_EXTI_IRQ;
	NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_Init_Structure);
	
}
void EXTI_Key_Config(void)
{
	RCC_AHB1PeriphClockCmd(KEY1_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	NVIC_Configurationg();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin =KEY1_PIN; 
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);
	
	SYSCFG_EXTILineConfig(KEY1_EXTI_PORTSOURCE,KEY1_EXTI_PINSOURCE);
	
	EXTI_InitStructure.EXTI_Line = KEY1_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
}
