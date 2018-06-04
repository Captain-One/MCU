#include "BasicTime.h"

static void TIMx_NVIC_Configurationg(void)
{
	NVIC_InitTypeDef NVIC_Init_Structure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_Init_Structure.NVIC_IRQChannel = BASIC_TIM_IRQn;
	NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_Init_Structure);
}

static void TIMx_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef BASIC_TIM_Init_Structure;
	RCC_APB1PeriphClockCmd(BASIC_TIM_CLK,ENABLE);
	
	BASIC_TIM_Init_Structure.TIM_Period =5000-1;
	BASIC_TIM_Init_Structure.TIM_Prescaler = 2500-1;
	TIM_TimeBaseInit(BASIC_TIM,&BASIC_TIM_Init_Structure);
	TIM_ClearFlag(BASIC_TIM,TIM_FLAG_Update);
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(BASIC_TIM,ENABLE);
}

void BASIC_TIM_Init(void)
{
	TIMx_NVIC_Configurationg();
	TIMx_Mode_Config();
}
