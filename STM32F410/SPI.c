#include "SPI.h"

uint32_t SPITimeOut;

void Flash_SPI_Config(void)
{
	RCC_AHB1PeriphClockCmd(FLASH_SPI_SCK_PortCLK|FLASH_SPI_MOSI_PortCLK|FLASH_SPI_MISO_PortCLK|FLASH_SPI_CS_PortCLK,ENABLE);
	RCC_APB2PeriphClockCmd(FLASH_SPI_CLK,ENABLE);
	
	GPIO_InitTypeDef gpio_initstruct;
	
	gpio_initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_initstruct.GPIO_OType = GPIO_OType_PP;
	gpio_initstruct.GPIO_Speed =GPIO_Fast_Speed;
	gpio_initstruct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_initstruct.GPIO_Mode =GPIO_Mode_OUT;
	gpio_initstruct.GPIO_Pin =FLASH_SPI_CS_Pin;
	GPIO_Init(FLASH_SPI_CS_Port,&gpio_initstruct);
	
	gpio_initstruct.GPIO_Mode =GPIO_Mode_AF;
	gpio_initstruct.GPIO_Pin =FLASH_SPI_SCK_Pin; 
	GPIO_Init(FLASH_SPI_SCK_Port,&gpio_initstruct);
	
	gpio_initstruct.GPIO_Pin =FLASH_SPI_MISO_Pin; 
	GPIO_Init(FLASH_SPI_MISO_Port,&gpio_initstruct);
	
	gpio_initstruct.GPIO_Pin =FLASH_SPI_MOSI_Pin; 
	GPIO_Init(FLASH_SPI_MOSI_Port,&gpio_initstruct);
	
	GPIO_PinAFConfig(FLASH_SPI_SCK_Port,FLASH_SPI_SCK_PinSource,FLASH_SPI_SCK_AF);
	GPIO_PinAFConfig(FLASH_SPI_MISO_Port,FLASH_SPI_MISO_PinSource,FLASH_SPI_MISO_AF);
	GPIO_PinAFConfig(FLASH_SPI_MOSI_Port,FLASH_SPI_MOSI_PinSource,FLASH_SPI_MOSI_AF);
	
	FLASH_SPI_CS_HIGH();
	
	SPI_InitTypeDef SPI_Initstrcut;
	SPI_Initstrcut.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_Initstrcut.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_Initstrcut.SPI_CPOL = SPI_CPOL_High;
  SPI_Initstrcut.SPI_CRCPolynomial = 7;
	SPI_Initstrcut.SPI_DataSize = SPI_DataSize_8b;
	SPI_Initstrcut.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Initstrcut.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Initstrcut.SPI_Mode = SPI_Mode_Master;
	SPI_Initstrcut.SPI_NSS  = SPI_NSS_Soft;
	SPI_Init(FLASH_SPI,&SPI_Initstrcut);
	
	SPI_Cmd(FLASH_SPI,ENABLE);
}

static uint32_t SPI_TIMEOUT_UserCallBack(uint8_t errorCode)
{
	SPI_ERROR("SPI µÈ´ý³¬Ê±£¡,SPI errorcode=%d ",errorCode);
	return 0;
}


uint8_t FLASH_SPI_SendByte(uint8_t byte)
{
	SPITimeOut=SPIT_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPI,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeOut--)==0)
		{
			return SPI_TIMEOUT_UserCallBack(0);
		}
	}
	SPI_I2S_SendData(FLASH_SPI,byte);
	SPITimeOut=SPIT_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPI,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeOut--)==0)
		{
			return SPI_TIMEOUT_UserCallBack(1);
		}
	}
	return SPI_I2S_ReceiveData(FLASH_SPI);
}


uint8_t FLASH_SPI_ReadByte(void)
{
	return FLASH_SPI_SendByte(Dummy_Byte);
}

uint32_t Read_SPI_Flash_ID(void)
{
	uint32_t Temp=0,Temp0=0,Temp1=0,Temp2=0;
	FLASH_SPI_CS_LOW();
	FLASH_SPI_SendByte(MX25LX_ReadIdentification);
	Temp0=FLASH_SPI_SendByte(Dummy_Byte);
	Temp1=FLASH_SPI_SendByte(Dummy_Byte);
	Temp2=FLASH_SPI_SendByte(Dummy_Byte);
	FLASH_SPI_CS_HIGH();
	Temp =(Temp0<<16) | (Temp1<<8) | (Temp2);
	return Temp;
}

uint32_t Read_SPI_Flash_ReadStatusReg(void)
{
	uint32_t Temp=0,Temp0=0,Temp1=0,Temp2=0;
	FLASH_SPI_CS_LOW();
	FLASH_SPI_SendByte(MX25LX_ReadStatusReg);
	Temp0=FLASH_SPI_SendByte(Dummy_Byte);
	Temp1=FLASH_SPI_SendByte(Dummy_Byte);
	Temp2=FLASH_SPI_SendByte(Dummy_Byte);
	FLASH_SPI_CS_HIGH();
	Temp =(Temp0<<16) | (Temp1<<8) | (Temp2);
	return Temp;
}

void SPI_Flash_WriteEnable(void)
{
	uint8_t Flash_Status=0;
	FLASH_SPI_CS_LOW();
	FLASH_SPI_SendByte(MX25LX_WriteEnable);
	FLASH_SPI_CS_HIGH();
	
	FLASH_SPI_SendByte(MX25LX_ReadStatusReg);
	SPITimeOut=SPIT_FLAG_TIMEOUT;
	do
	{
		Flash_Status=FLASH_SPI_SendByte(Dummy_Byte);
		if(SPITimeOut==0)
		{
			SPI_TIMEOUT_UserCallBack(4);
			return;
		}
	}
	while((Flash_Status&WEL_Flag)!=0x02);
	FLASH_SPI_CS_HIGH();
}



void SPI_Flash_WaitForWriteEnd(void)
{
	uint8_t Flash_Status=0;
	FLASH_SPI_CS_LOW();
	FLASH_SPI_SendByte(MX25LX_ReadStatusReg);
	SPITimeOut=SPIT_FLAG_TIMEOUT;
	do
	{
		Flash_Status=FLASH_SPI_SendByte(Dummy_Byte);
		if(SPITimeOut==0)
		{
			SPI_TIMEOUT_UserCallBack(4);
			return;
		}
	}
	while((Flash_Status&WIP_Flag)==SET);
	FLASH_SPI_CS_HIGH();
}

void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	SPI_Flash_WriteEnable();
	//SPI_Flash_WaitForWriteEnd();
	FLASH_SPI_CS_LOW();
	FLASH_SPI_SendByte(MX25LX_SectorErase);
	FLASH_SPI_SendByte((SectorAddr&0xff0000)>>16);
	FLASH_SPI_SendByte((SectorAddr&0xff00)>>8);
	FLASH_SPI_SendByte(SectorAddr&0xff);
	FLASH_SPI_CS_HIGH();
	SPI_Flash_WaitForWriteEnd();
}

void SPI_FLASH_PageWrite(uint8_t *pbuff,uint32_t WriteAddr,uint16_t NumWriteToByte)
{
	SPI_Flash_WriteEnable();
	FLASH_SPI_CS_LOW();
	FLASH_SPI_SendByte(MX25LX_PageProgram);
	FLASH_SPI_SendByte((WriteAddr&0xff0000)>>16);
	FLASH_SPI_SendByte((WriteAddr&0xff00)>>8);
	FLASH_SPI_SendByte(WriteAddr&0xff);
	if(NumWriteToByte>SPI_FLASH_PerWritePageSize)
	{
		NumWriteToByte=SPI_FLASH_PerWritePageSize;
		SPI_ERROR("Write to flash num too large!");
	}
	while(NumWriteToByte--)
	{
		FLASH_SPI_SendByte(*pbuff);
		pbuff++;
	}
	FLASH_SPI_CS_HIGH();
	SPI_Flash_WaitForWriteEnd();
}

void SPI_FLASH_BufferWrite(uint8_t *pbuff,uint32_t WriteAddr,uint16_t NumWriteToByte)
{
	uint8_t NumOfPage=0,NumOfSingle=0,Addr=0,count=0,temp=0;
	
	Addr=WriteAddr%SPI_FLASH_PageSize;
	count= SPI_FLASH_PageSize-Addr;
	NumOfPage=NumWriteToByte/SPI_FLASH_PageSize;
	NumOfSingle=NumWriteToByte%SPI_FLASH_PageSize;
	
	if(Addr==0)
	{
		if(NumOfPage==0)
		{
			SPI_FLASH_PageWrite(pbuff,WriteAddr,NumWriteToByte);
		}
		else
		{
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pbuff,WriteAddr,SPI_FLASH_PageSize);
				pbuff +=SPI_FLASH_PageSize;
				WriteAddr +=SPI_FLASH_PageSize;
			}
			SPI_FLASH_PageWrite(pbuff,WriteAddr,NumOfSingle);
		}
	}
	else
	{
		if(NumOfPage==0)
		{
			if(NumOfSingle>count)
			{
				temp=NumOfSingle-count;
				SPI_FLASH_PageWrite(pbuff,WriteAddr,count);
				pbuff +=count;
				WriteAddr +=count;
				SPI_FLASH_PageWrite(pbuff,WriteAddr,temp);
			}
			else
			{
			  SPI_FLASH_PageWrite(pbuff,WriteAddr,NumWriteToByte);
			}
		}
		else
		{
			NumWriteToByte -=count;
	    NumOfPage=NumWriteToByte/SPI_FLASH_PageSize;
	    NumOfSingle=NumWriteToByte%SPI_FLASH_PageSize; 
			SPI_FLASH_PageWrite(pbuff,WriteAddr,count);
			pbuff +=count;
			WriteAddr +=count;
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pbuff,WriteAddr,SPI_FLASH_PageSize);
				pbuff +=SPI_FLASH_PageSize;
				WriteAddr +=SPI_FLASH_PageSize;
			}
			if(NumOfSingle!=0)
			{SPI_FLASH_PageWrite(pbuff,WriteAddr,NumOfSingle);}
		}
	}
}

void SPI_FLASH_BufferRead(uint8_t *pbuff,uint32_t ReadAddr,uint16_t NumReadToByte)
{
	FLASH_SPI_CS_LOW();
	//SPI_Flash_WaitForWriteEnd();
	FLASH_SPI_SendByte(MX25LX_ReadData);
	FLASH_SPI_SendByte((ReadAddr&0xff0000)>>16);
	FLASH_SPI_SendByte((ReadAddr&0xff00)>>8);
	FLASH_SPI_SendByte(ReadAddr&0xff);
	while(NumReadToByte--)
	{
		*pbuff=FLASH_SPI_SendByte(Dummy_Byte);
		 pbuff++;
	}
	FLASH_SPI_CS_HIGH();
}


