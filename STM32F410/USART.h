#ifndef __USART_H
#define __USART_H

#include<stm32f4xx_usart.h>

#define USART_DEBUG 										USART2
#define USART_DEBUG_CLK 								RCC_APB1Periph_USART2
#define USART_DEBUG_BAUTRATE  					115200

#define USART_DEBUG_TX_PORT   					GPIOA
#define USART_DEBUG_TX_PIN    					GPIO_Pin_2
#define USART_DEBUG_TX_CLK        			RCC_AHB1Periph_GPIOA
#define USART_DEBUG_TX_AF								GPIO_AF_USART2
#define USART_DEBUG_TX_PINSOURCE				GPIO_PinSource2

#define USART_DEBUG_RX_PORT   					GPIOA
#define USART_DEBUG_RX_PIN    					GPIO_Pin_3
#define USART_DEBUG_RX_CLK        			RCC_AHB1Periph_GPIOA
#define USART_DEBUG_RX_AF								GPIO_AF_USART2
#define USART_DEBUG_RX_PINSOURCE				GPIO_PinSource3

#define USART_DEBUG_IRQn								USART2_IRQn
#define USART_DEBUG_IRQnHandler					USART2_IRQHandler

#define USART_DEBUG_DMA_CLK             RCC_AHB1Periph_DMA1
#define USART_DEBUG_DMA_STREAM    			DMA1_Stream6
#define USART_DEBUG_DMA_CHANNEL   			DMA_Channel_4
#define USART_DEBUG_DMA_DR_ADDR         USART2_BASE+0x04
#define USART_DEBUG_DMA_SEND_BUFFSIZE   500

void NVIC_Configurationg(void);
void DEBUG_USART_Config(void);
void USARTx_SendByte(USART_TypeDef* pUSARTx,uint8_t data);
void USARTx_SendString(USART_TypeDef* pUSARTx,char *str);
void USARTx_SendHalfWord(USART_TypeDef* pUSARTx,uint16_t data);
void USART_DEBUG_DMA_Config(void);

#endif
