#ifndef __SPI_H
#define __SPI_H

#include<stm32f4xx_spi.h>
#include<stdio.h>

#define _DEBUG_   													1

#if _DEBUG_
#define SPI_ERROR(...)										printf(__VA_ARGS__)
#else
#define SPI_ERROR(...)
#endif

#define FLASH_SPI                     			SPI1
#define FLASH_SPI_CLK                       RCC_APB2Periph_SPI1

#define FLASH_SPI_SCK_Port          				GPIOA
#define FLASH_SPI_SCK_Pin           				GPIO_Pin_5
#define FLASH_SPI_SCK_PortCLK       				RCC_AHB1Periph_GPIOA
#define FLASH_SPI_SCK_AF                    GPIO_AF_SPI1
#define FLASH_SPI_SCK_PinSource     				GPIO_PinSource5

#define FLASH_SPI_MOSI_Port         				GPIOA
#define FLASH_SPI_MOSI_Pin          				GPIO_Pin_7
#define FLASH_SPI_MOSI_PortCLK    					RCC_AHB1Periph_GPIOA
#define FLASH_SPI_MOSI_AF                   GPIO_AF_SPI1
#define FLASH_SPI_MOSI_PinSource    				GPIO_PinSource7

#define FLASH_SPI_MISO_Port         				GPIOA
#define FLASH_SPI_MISO_Pin          				GPIO_Pin_6
#define FLASH_SPI_MISO_PortCLK    					RCC_AHB1Periph_GPIOA
#define FLASH_SPI_MISO_AF                   GPIO_AF_SPI1
#define FLASH_SPI_MISO_PinSource    				GPIO_PinSource6

#define FLASH_SPI_CS_Port               		GPIOB
#define FLASH_SPI_CS_Pin                		GPIO_Pin_6
#define FLASH_SPI_CS_PortCLK        				RCC_AHB1Periph_GPIOB

#define FLASH_SPI_CS_LOW()       				    {FLASH_SPI_CS_Port->BSRRH=FLASH_SPI_CS_Pin;}
#define FLASH_SPI_CS_HIGH()       				  {FLASH_SPI_CS_Port->BSRRL=FLASH_SPI_CS_Pin;}

#define Dummy_Byte                          0xFF

#define W25X_WriteEnable                    0x06
#define W25X_ReadStatusReg              		0x04
#define W25X_WriteStatusReg              		0x05
#define W25X_ReadData                       0x01
#define W25X_FastReadData                   0x03
#define W25X_FastReadDual                   0x0B
#define W25X_PageProgram                    0x02
#define W25X_BlockErase                     0xD8
#define W25X_SectorErase                    0x20
#define W25X_ChipErase                      0xC7
#define W25X_PowerDown                      0xB9
#define W25X_ReleasePowerDown       				0xAB
#define W25X_DeviceID                       0xAB
#define W25X_ManufactDeviceID       				0x90
#define W25X_JedecDeviceID              		0x9F

#define W25X_sFLASH_ID                      0XEF4018

#define WIP_Flag														0x01
#define WEL_Flag														0x02

#define MX25LX_WriteEnable                  0x06
#define MX25LX_WriteDisable                 0x04
#define MX25LX_ReadStatusReg                0x05
#define MX25LX_SectorErase                  0x20
#define MX25LX_PageProgram                  0x02
#define MX25LX_ReadData                     0x03
#define MX25LX_ReadConfigReg                0x15
#define MX25LX_WriteStatOrConfigReg         0x01     //0x01,value,value
#define MX25LX_WriteProtSlectReg            0x68
#define MX25LX_EnableOPI                    0x35
#define MX25LX_ResetOPI                     0xF5
#define MX25LX_SuspendPrOrEr                0xB0
#define MX25LX_ResumePrOrEr                 0x30
#define MX25LX_PowerDown                    0xB9
#define MX25LX_ReleasPowerDown              0xAB
#define MX25LX_SetBurstLength               0xC0
#define MX25LX_ReadFastBootReg              0x16
#define MX25LX_WriteFastBootReg             0x17
#define MX25LX_EraseFastBootReg             0x18
#define MX25LX_ReadIdentification           0x9F
#define MX25LX_EnterSecuredOPT              0xB1
#define MX25LX_ExitSecuredOPT		            0xC1
#define MX25LX_ReadSecurityReg              0x2B
#define MX25LX_WriteSecurityReg             0x2F
#define MX25LX_GangBlockLock                0x7E
#define MX25LX_GangBlockUnlock              0x98 
#define MX25LX_WriteLockReg                 0x2C
#define MX25LX_ReadLockReg                  0x2D
#define MX25LX_WritePasswordReg             0x28
#define MX25LX_ReadPasswordReg              0x27
#define MX25LX_PasswordUnlock               0x29
#define MX25LX_SPBbitProgram                0xE3 //0xE1,add1,add2,add3,add4
#define MX25LX_AllSPBbitErase               0xE4
#define MX25LX_ReadSPBStatus                0xE2 //0xE1,add1,add2,add3,add4   
#define MX25LX_SPBLockSet                   0xA6
#define MX25LX_ReadSPBLockReg               0xA7                                                 
#define MX25LX_WriteDPBLockReg              0xE1 //0xE1,add1,add2,add3,add4
#define MX25LX_ReadDPBLockReg               0xE0 //0xE0,add1,add2,add3,add4
#define MX25LX_RsetEnable                   0x66
#define MX25LX_RsetMemory                   0x99

#define MX25LX_sFLASH_ID                    0XC22018

#define SPIT_FLAG_TIMEOUT  									((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT  									((uint32_t)10*SPIT_FLAG_TIMEOUT)

#define SPI_FLASH_PageSize									256
#define SPI_FLASH_PerWritePageSize          256

void Flash_SPI_Config(void);
static uint32_t SPI_TIMEOUT_UserCallBack(uint8_t errorCode);
uint8_t FLASH_SPI_SendByte(uint8_t byte);
uint8_t FLASH_SPI_ReadByte(void);
uint32_t Read_SPI_Flash_ID(void);
void SPI_Flash_WriteEnable(void);
void SPI_Flash_WaitForWriteEnd(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_PageWrite(uint8_t *pbuff,uint32_t WriteAddr,uint16_t NumWriteToByte);
void SPI_FLASH_BufferWrite(uint8_t *pbuff,uint32_t WriteAddr,uint16_t NumWriteToByte);
void SPI_FLASH_BufferRead(uint8_t *pbuff,uint32_t ReadAddr,uint16_t NumReadToByte);
uint32_t Read_SPI_Flash_ReadStatusReg(void);
#endif

