#include<I2C.h>

uint32_t I2CTimeOut;

static void I2C_GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_CLK|EEPROM_I2C_SDA_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN; 
	GPIO_Init(EEPROM_I2C_SCL_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =EEPROM_I2C_SDA_PIN; 
	GPIO_Init(EEPROM_I2C_SDA_PORT,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(EEPROM_I2C_SCL_PORT,EEPROM_I2C_SCL_PINSOURCE,EEPROM_I2C_SCL_AF);
	GPIO_PinAFConfig(EEPROM_I2C_SDA_PORT,EEPROM_I2C_SDA_PINSOURCE,EEPROM_I2C_SDA_AF);
}

static void I2C_Mode_Config(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = EEPROM_I2C_SPEED;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = EEPROM_I2C_OWNADDR;
	
	I2C_Init(EEPROM_I2C,&I2C_InitStructure);
	I2C_Cmd(EEPROM_I2C,ENABLE);
}

void EEPROM_I2C_Init(void)
{
	I2C_GPIO_Config();
	I2C_Mode_Config();
}

static uint32_t I2C_TIMEOUT_UserCallBack(uint8_t errorCode)
{
	EEPROM_ERROR("I2C 等待超时！,I2C errorcode=%d",errorCode);
	return 0;
}

uint32_t EEPROM_I2C_ByteWrite(uint8_t * pbuff,uint8_t WriteAddr)
{
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(0);
		}
	}
	
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(1);
		}
	}
	
	I2C_SendData(EEPROM_I2C,WriteAddr);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(2);
		}
	}
	
	I2C_SendData(EEPROM_I2C,*pbuff);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(3);
		}
	}
	
	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);
	return 1;
}

uint32_t EEPROM_I2C_BytesWrite(uint8_t * pbuff,uint8_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t i;
	uint32_t res;
	for(i=0;i<NumByteToWrite;i++)
	{
		EEPROM_I2C_WaitEepromStandbyeState();
		res=EEPROM_I2C_ByteWrite(pbuff++,WriteAddr++);
	}
	return res;
}

#define MAX_TRIAL_NUMBER 				300

static uint8_t EEPROM_I2C_WaitEepromStandbyeState(void)
{
  __IO uint16_t tmpSR1=0;
	__IO uint32_t EETrials=0;
	
	I2CTimeOut=I2CT_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_BUSY))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(20);
		}
	}
	
	while(1)
	{
		I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	  I2CTimeOut=I2CT_FLAG_TIMEOUT;
	  while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	  {
		  if((I2CTimeOut--)==0)
		  {
			  return I2C_TIMEOUT_UserCallBack(21);
		  }
	  }
		
		I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	  I2CTimeOut=I2CT_LONG_TIMEOUT;
	  do
		{
			tmpSR1=EEPROM_I2C->SR1;
			if((I2CTimeOut--)==0)
		  {
			  return I2C_TIMEOUT_UserCallBack(22);
		  }
		}
		while((tmpSR1&(I2C_SR1_ADDR|I2C_SR1_AF))==0);
		
		if(tmpSR1&I2C_SR1_ADDR)
		{
			(void)EEPROM_I2C->SR2;
			I2C_GenerateSTOP(EEPROM_I2C,ENABLE);
	    return 1;
		}
		else
		{
			I2C_ClearFlag(EEPROM_I2C,I2C_FLAG_AF);
			return 1;
		}
		
		if(EETrials++==MAX_TRIAL_NUMBER)
		{
			return I2C_TIMEOUT_UserCallBack(23);
		}
	}
}

uint8_t EEPROM_I2C_PageWrite(uint8_t *pbuffer,uint8_t writeaddr,uint8_t Num)
{
	I2CTimeOut=I2CT_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_BUSY))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(4);
		}
	}
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(5);
		}
	}
	
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(6);
		}
	}
	
	I2C_SendData(EEPROM_I2C,writeaddr);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(7);
		}
	}
	while(Num--)
	{
		I2C_SendData(EEPROM_I2C,*pbuffer);
		pbuffer++;
		I2CTimeOut=I2CT_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if((I2CTimeOut--)==0)
			{
				return I2C_TIMEOUT_UserCallBack(8);
			}
		}
	}
	
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);	
	return 1;
}

#define I2C_Page_Size     8

void EEPROM_I2C_BufferWrite(uint8_t *pbuffer,uint8_t writeaddr,uint16_t Num)
{
	uint8_t numofpage=0,numofsingle=0,addr=0,count=0;
	
	addr = writeaddr%I2C_Page_Size;						/***********判断是否页对齐***************/
	count = I2C_Page_Size-addr;								/***********还差多少个字对齐************/
	numofpage = Num/I2C_Page_Size;						/***********多少页***********/
	numofsingle = Num%I2C_Page_Size;					/***********不满一页的字数***********/
	if(addr==0)
	{
		/***********不满一页***********/
		if(numofpage==0)
		{
			EEPROM_I2C_PageWrite(pbuffer,writeaddr,numofsingle);
			EEPROM_I2C_WaitEepromStandbyeState();
		}
		else
		{
			/***********先写页***********/
			while(numofpage--)
			{
				EEPROM_I2C_PageWrite(pbuffer,writeaddr,I2C_Page_Size);
			  EEPROM_I2C_WaitEepromStandbyeState();
				writeaddr += I2C_Page_Size;
				pbuffer   += I2C_Page_Size;
			}
			/***********写剩下的***********/
			if(numofsingle!=0)
			{
				EEPROM_I2C_PageWrite(pbuffer,writeaddr,numofsingle);
				EEPROM_I2C_WaitEepromStandbyeState();
			}
		}
	}
	else/***********页不对齐***********/
	{
		/***********不满一页***********/
		if(numofpage==0)
		{
			EEPROM_I2C_PageWrite(pbuffer,writeaddr,numofsingle);
			EEPROM_I2C_WaitEepromStandbyeState();
		}
		else
		{
			/***********对齐处理***********/
			Num -=count;
			numofpage = Num/I2C_Page_Size;						/***********多少页***********/
	    numofsingle = Num%I2C_Page_Size;					/***********不满一页的字数***********/
			if(count!=0)
			{
				EEPROM_I2C_PageWrite(pbuffer,writeaddr,count);
			  EEPROM_I2C_WaitEepromStandbyeState();
				writeaddr +=count;
				pbuffer +=count;
			}
			while(numofpage--)
			{
				EEPROM_I2C_PageWrite(pbuffer,writeaddr,I2C_Page_Size);
			  EEPROM_I2C_WaitEepromStandbyeState();
				writeaddr += I2C_Page_Size;
				pbuffer   += I2C_Page_Size;
			}
			if(numofsingle!=0)
			{
				EEPROM_I2C_PageWrite(pbuffer,writeaddr,numofsingle);
				EEPROM_I2C_WaitEepromStandbyeState();
			}
		}
	}
}



uint8_t EEPROM_I2C_BufferRead(uint8_t *pbuffer,uint8_t readaddr,uint16_t Num)
{
	I2CTimeOut=I2CT_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C,I2C_FLAG_BUSY))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(9);
		}
	}
	
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(10);
		}
	}
	
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(11);
		}
	}
	
	I2C_Cmd(EEPROM_I2C,ENABLE);
	I2C_SendData(EEPROM_I2C,readaddr);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(12);
		}
	}
	
	I2C_GenerateSTART(EEPROM_I2C,ENABLE);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(13);
		}
	}
	
	I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Receiver);
	I2CTimeOut=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((I2CTimeOut--)==0)
		{
			return I2C_TIMEOUT_UserCallBack(14);
		}
	}
	
	while(Num)
	{
		if(Num==1)
		{
			I2C_AcknowledgeConfig(EEPROM_I2C,DISABLE);
			I2C_GenerateSTOP(EEPROM_I2C,ENABLE);
		}
		I2CTimeOut=I2CT_LONG_TIMEOUT;
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
		{
			if((I2CTimeOut--)==0)
			{
				return I2C_TIMEOUT_UserCallBack(3);
			}
		}
		*pbuffer = I2C_ReceiveData(EEPROM_I2C);
		pbuffer++;
		Num--;
	}
	I2C_AcknowledgeConfig(EEPROM_I2C,ENABLE);
	return 1;
}
	

