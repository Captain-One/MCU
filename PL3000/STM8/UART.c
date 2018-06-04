#include "UART.h"
#include <stdio.h>

void DEBUG_USART_Config(void)
{
/*************GPIO Config*************/	
  GPIO_Init(USART_DEBUG_TX_PORT,USART_DEBUG_TX_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(USART_DEBUG_RX_PORT,USART_DEBUG_RX_PIN, GPIO_MODE_IN_PU_NO_IT);	
/*************USART Config*************/
  
  UART2_Init(USART_DEBUG_BAUTRATE, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, 
             UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
  UART2_Cmd(ENABLE);
}

void USARTx_SendByte(uint8_t data)
{
 UART2_SendData8(data);
 while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);  
}


void USARTx_SendFrame(uint8_t data[],uint8_t data_length)
{
  for(int i=0;i<data_length;i++)
  {
    USARTx_SendByte(data[i]);
    while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
  }
}

void USARTx_SendString(char *str)
{
  unsigned int i=0;
  do
  {
    USARTx_SendByte(*(str+i));
    i++;
  }while(*(str+i)!='\0');
  while(UART2_GetFlagStatus(UART2_FLAG_TC)==RESET);   
}

void USARTx_SendHalfWord(uint16_t data)
{
  uint8_t temp_h,temp_l;
  temp_h=(data&0xff00)>>8;
  temp_l=data&0xff;
  UART2_SendData8(temp_h);
  while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
  UART2_SendData8(temp_l);
  while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
}

int fputc(int ch,FILE *f)
{
  UART2_SendData8(ch);
  while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
  return (ch);
}

int fgetc(FILE *f)
{
  while(UART2_GetFlagStatus(UART2_FLAG_RXNE)==RESET);
  return (int)UART2_ReceiveData8();
}


