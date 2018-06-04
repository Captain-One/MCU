/**
  ******************************************************************************
  * @file    LED.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define LED Pin, PORT and Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

#include <stm8s.h>
#include "stm8s_gpio.h"

#define USART_DEBUG 						UART2
#define USART_DEBUG_BAUTRATE  					115200

#define USART_DEBUG_TX_PORT   					GPIOD
#define USART_DEBUG_TX_PIN    					GPIO_PIN_5


#define USART_DEBUG_RX_PORT   					GPIOD
#define USART_DEBUG_RX_PIN    					GPIO_PIN_6

#define USART_DEBUG_IRQn					USART2_IRQn
#define USART_DEBUG_IRQnHandler					USART2_IRQHandler

void DEBUG_USART_Config(void);
void USARTx_SendByte(uint8_t data);
void USARTx_SendString(char *str);
void USARTx_SendHalfWord(uint16_t data);
void USARTx_SendFrame(uint8_t data[],uint8_t data_length);

#endif
