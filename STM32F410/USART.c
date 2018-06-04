#include<USART.h>
#include<stdio.h>

extern char SendBuff[500];

static void NVIC_Configurationg(void)
{
	NVIC_InitTypeDef NVIC_Init_Structure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_Init_Structure.NVIC_IRQChannel = USART_DEBUG_IRQn;
	NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_Init_Structure);
}

void DEBUG_USART_Config(void)
{
	RCC_AHB1PeriphClockCmd(USART_DEBUG_TX_CLK|USART_DEBUG_RX_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(USART_DEBUG_CLK,ENABLE);
	
 /*************GPIO Config*************/	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Fast_Speed;
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =USART_DEBUG_TX_PIN; 
	GPIO_Init(USART_DEBUG_TX_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =USART_DEBUG_RX_PIN; 
	GPIO_Init(USART_DEBUG_RX_PORT,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(USART_DEBUG_TX_PORT,USART_DEBUG_TX_PINSOURCE,USART_DEBUG_TX_AF);
	GPIO_PinAFConfig(USART_DEBUG_RX_PORT,USART_DEBUG_RX_PINSOURCE,USART_DEBUG_RX_AF);
	
	/*************USART Config*************/
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate =USART_DEBUG_BAUTRATE;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity =USART_Parity_No ;
	USART_InitStructure.USART_StopBits =USART_StopBits_1;
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;
	
	USART_Init(USART_DEBUG,&USART_InitStructure);
	//NVIC_Configurationg();
	//USART_ITConfig(USART_DEBUG,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART_DEBUG,ENABLE);
}

void USARTx_SendByte(USART_TypeDef* pUSARTx,uint8_t data)
{
	 USART_SendData(pUSARTx,data);
	 while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);  //数据寄存器为空，移位寄存器不为空
}

void USARTx_SendString(USART_TypeDef* pUSARTx,char *str)
{
	unsigned int i=0;
	do
	{
		USARTx_SendByte(pUSARTx,*(str+i));
		i++;
	}while(*(str+i)!='\0');
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);   //移位寄存器数据发送完，即所有数据发送完
}

void USARTx_SendHalfWord(USART_TypeDef* pUSARTx,uint16_t data)
{
	uint8_t temp_h,temp_l;
	temp_h=(data&0xff00)>>8;
	temp_l=data&0xff;
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
	USART_SendData(pUSARTx,temp_l);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

int fputc(int ch,FILE *f)
{
	USART_SendData(USART_DEBUG,ch);
  while(USART_GetFlagStatus(USART_DEBUG,USART_FLAG_TXE)==RESET);
	return (ch);
}

int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART_DEBUG,USART_FLAG_RXNE)==RESET);
	return (int)USART_ReceiveData(USART_DEBUG);
}


void USART_DEBUG_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(USART_DEBUG_DMA_CLK,ENABLE);
	DMA_DeInit(USART_DEBUG_DMA_STREAM);
	while(DMA_GetCmdStatus(USART_DEBUG_DMA_STREAM)!=DISABLE){
	}
	
	DMA_InitStructure.DMA_Channel=USART_DEBUG_DMA_CHANNEL;
	DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize=USART_DEBUG_DMA_SEND_BUFFSIZE;
	DMA_InitStructure.DMA_FIFOMode=DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_Memory0BaseAddr =(u32)SendBuff;
	DMA_InitStructure.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr=USART_DEBUG_DMA_DR_ADDR;
	DMA_InitStructure.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
	
	DMA_Init(USART_DEBUG_DMA_STREAM,&DMA_InitStructure);
	DMA_ClearFlag(USART_DEBUG_DMA_STREAM,DMA_FLAG_TCIF0);
	
	DMA_Cmd(USART_DEBUG_DMA_STREAM, ENABLE);
	while((DMA_GetCmdStatus(USART_DEBUG_DMA_STREAM)!=ENABLE)){
	}
}

