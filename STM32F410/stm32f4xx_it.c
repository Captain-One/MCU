
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"

extern uint32_t TimeDelya;

void KEY1_EXTI_IRQHandler(void)
{
	if(EXTI_GetITStatus(KEY1_EXTI_LINE)!=RESET)
	{
		LED_RED_TOG;
		EXTI_ClearITPendingBit(KEY1_EXTI_LINE);
	}
}

void SysTick_Handler(void)
{
	if(TimeDelya!=0)
	   TimeDelya--;
}

void USART_DEBUG_IRQnHandler(void)
{
	uint8_t temp;
	if(USART_GetITStatus(USART_DEBUG,USART_IT_RXNE)!=RESET)
	{
		temp=USART_ReceiveData(USART_DEBUG);
		USART_SendData(USART_DEBUG,temp);
	}
}

void BASIC_TIM_IRQHandler(void)
{
	if(TIM_GetITStatus(BASIC_TIM,TIM_IT_Update)!=RESET)
	{
		LED_RED_TOG;
		TIM_ClearITPendingBit(BASIC_TIM,TIM_IT_Update);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
