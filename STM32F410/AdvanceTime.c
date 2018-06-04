#include "AdvanceTime.h"

static void TIMx_GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(ADVANCE_TIM_OCPWM_PIN_CLK|ADVANCE_TIM_OCNPWM_PIN_CLK|ADVANCE_TIM_BKIN_PIN_CLK,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Fast_Speed;
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =ADVANCE_TIM_OCPWM_PIN; 
	GPIO_Init(ADVANCE_TIM_OCPWM_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =ADVANCE_TIM_OCNPWM_PIN; 
	GPIO_Init(ADVANCE_TIM_OCNPWM_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =ADVANCE_TIM_BKIN_PIN; 
	GPIO_Init(ADVANCE_TIM_BKIN_PORT,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(ADVANCE_TIM_OCPWM_PORT,ADVANCE_TIM_OCPWM_PINSOURCE,ADVANCE_TIM_OCPWM_AF);
	GPIO_PinAFConfig(ADVANCE_TIM_OCNPWM_PORT,ADVANCE_TIM_OCNPWM_PINSOURCE,ADVANCE_TIM_OCNPWM_AF);
	GPIO_PinAFConfig(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PINSOURCE,ADVANCE_TIM_BKIN_AF);
}

static void TIMx_Mode_Config(void)
{
	
	TIM_TimeBaseInitTypeDef   TIM_TimeBase_Init_Structure;
	TIM_OCInitTypeDef         TIM_OC_Init_Structure;
	TIM_BDTRInitTypeDef       TIM_BDTR_TIM_Init_Structure;
	
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK,ENABLE);
	
	TIM_TimeBase_Init_Structure.TIM_ClockDivision 		=TIM_CKD_DIV1; 
	TIM_TimeBase_Init_Structure.TIM_CounterMode   		=TIM_CounterMode_Up;
	TIM_TimeBase_Init_Structure.TIM_Period        		=1024-1;
	TIM_TimeBase_Init_Structure.TIM_Prescaler   	 		=250-1;
	TIM_TimeBase_Init_Structure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(ADVANCE_TIM,&TIM_TimeBase_Init_Structure);
	
	TIM_OC_Init_Structure.TIM_OCIdleState	 =TIM_OCIdleState_Set;
	TIM_OC_Init_Structure.TIM_OCMode			 =TIM_OCMode_PWM1;
	TIM_OC_Init_Structure.TIM_OCNIdleState =TIM_OCIdleState_Reset;
	TIM_OC_Init_Structure.TIM_OCNPolarity	 =TIM_OCNPolarity_High;
	TIM_OC_Init_Structure.TIM_OCPolarity   =TIM_OCPolarity_High;
	TIM_OC_Init_Structure.TIM_OutputNState =TIM_OutputNState_Enable;
	TIM_OC_Init_Structure.TIM_OutputState  =TIM_OutputState_Enable;
	TIM_OC_Init_Structure.TIM_Pulse        =ChannelPulse;
	TIM_OC1Init(ADVANCE_TIM,&TIM_OC_Init_Structure);
	
	TIM_OC1PreloadConfig(ADVANCE_TIM,TIM_OCPreload_Enable);
	
	TIM_BDTR_TIM_Init_Structure.TIM_AutomaticOutput =TIM_AutomaticOutput_Enable;
	TIM_BDTR_TIM_Init_Structure.TIM_Break           =TIM_Break_Enable;
	TIM_BDTR_TIM_Init_Structure.TIM_BreakPolarity   =TIM_BreakPolarity_Low;
	TIM_BDTR_TIM_Init_Structure.TIM_DeadTime        =11;
	TIM_BDTR_TIM_Init_Structure.TIM_LOCKLevel       =TIM_LOCKLevel_1; 
	TIM_BDTR_TIM_Init_Structure.TIM_OSSIState       =TIM_OSSIState_Enable;
	TIM_BDTR_TIM_Init_Structure.TIM_OSSRState       =TIM_OSSRState_Enable;
	TIM_BDTRConfig(ADVANCE_TIM,&TIM_BDTR_TIM_Init_Structure);
	
	//TIM_ClearFlag(ADVANCE_TIM,TIM_FLAG_Update);
	//TIM_ITConfig(ADVANCE_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(ADVANCE_TIM,ENABLE);
	TIM_CtrlPWMOutputs(ADVANCE_TIM,ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	 TIMx_GPIO_Config();
	 TIMx_Mode_Config();
}
