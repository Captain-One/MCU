#ifndef __I2C_H
#define __I2C_H

#include<stm32f4xx_i2c.h>
#include<stdio.h>

#define _DEBUG_   								1

#if _DEBUG_
#define EEPROM_ERROR(...)					printf(__VA_ARGS__)
#else
#define EEPROM_ERROR(...)
#endif

#define EEPROM_I2C_SPEED					400000
#define EEPROM_I2C_OWNADDR        0x0A
#define EEPROM_ADDR 							0x00

#define EEPROM_I2C 								I2C1
#define EEPROM_I2C_CLK 						RCC_APB1Periph_USART2
//#define EEPROM_WritAddr 					0x01

#define EEPROM_I2C_SCL_PORT 			GPIOB
#define EEPROM_I2C_SCL_PIN  			GPIO_Pin_6
#define EEPROM_I2C_SCL_CLK 		    RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SCL_AF       	GPIO_AF_I2C1
#define EEPROM_I2C_SCL_PINSOURCE 	GPIO_PinSource6

#define EEPROM_I2C_SDA_PORT 			GPIOB
#define EEPROM_I2C_SDA_PIN  			GPIO_Pin_7
#define EEPROM_I2C_SDA_CLK    		RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SDA_AF       	GPIO_AF_I2C1
#define EEPROM_I2C_SDA_PINSOURCE 	GPIO_PinSource7

#define EEPROM_I2C_EV_IRQn			  I2C1_EV_IRQn
#define EEPROM_I2C_ER_IRQn				I2C1_ER_IRQn

/*
#define USART_DEBUG_DMA_Stream    DMA1_Stream6
#define USART_DEBUG_DMA_Channel   DMA_Channel_4
#define USART_DEBUG_DMA_DR_Addr   USART2_BASE+0x04
#define USART_DEBUG_DMA_BuffSize  500
*/

#define I2CT_FLAG_TIMEOUT  				((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT  				((uint32_t)10*I2CT_FLAG_TIMEOUT)



static void I2C_GPIO_Config(void);
static void I2C_Mode_Config(void);
void EEPROM_I2C_Init(void);
static uint32_t I2C_TIMEOUT_UserCallBack(uint8_t errorCode);
uint32_t EEPROM_I2C_ByteWrite(uint8_t * pbuff,uint8_t WriteAddr);
uint32_t EEPROM_I2C_BytesWrite(uint8_t * pbuff,uint8_t WriteAddr,uint16_t NumByteToWrite);
static uint8_t EEPROM_I2C_WaitEepromStandbyeState(void);
uint8_t EEPROM_I2C_PageWrite(uint8_t *pbuffer,uint8_t writeaddr,uint8_t Num);
void EEPROM_I2C_BufferWrite(uint8_t *pbuffer,uint8_t writeaddr,uint16_t Num);
uint8_t EEPROM_I2C_BufferRead(uint8_t *pbuffer,uint8_t readaddr,uint16_t Num);

#endif
