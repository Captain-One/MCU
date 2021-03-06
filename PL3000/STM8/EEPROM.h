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

#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm8s.h"
#include <stdio.h>

#define _DEBUG_   			0

#if _DEBUG_
#define EEPROM_ERROR(...)		printf(__VA_ARGS__)
#else
#define EEPROM_ERROR(...)
#endif

#define EEPROM_EEROR_SAK_TIMEOUT        1
#define EEPROM_EEROR_NoSAK_TIMEOUT      2

#define EEPORM_UNIO_PORT                GPIOC
#define EEPROM_UNIO_PIN                 GPIO_PIN_2

#define EEPROM_DEVICE_ADDR              0xA0
#define DATA_ADDR                       0x0000

#define EEPROM_WAIT                     0x14

#define EEPROM_STARTHEADER              0x55

#define CMD_READ                        0x03
#define CMD_CRRD                        0x06
#define CMD_WRITE                       0x6C
#define CMD_WREN                        0x96
#define CMD_WRDI                        0x91
#define CMD_RDSR                        0x05
#define CMD_WRSR                        0x6E
#define CMD_ERAL                        0x6D
#define CMD_SETAL                       0x67

static uint32_t I2C_TIMEOUT_UserCallBack(uint8_t errorCode);
void EEPROM_GPIO_Init(void);
void EEPROM_MAK(void) ;
void EEPROM_NoMAK(void);
void EEPROM_SAK(void);
void EEPROM_NoSAK(void);
void EEPROM_Write_Byte(uint8_t write_data);
uint8_t EEPROM_Read_Byte(void);
void EEPROM_StartHeader(void);
void EEPROM_Read_Data(uint8_t *data_arry, uint16_t read_addr, uint8_t read_num);
void EEPROM_Write_Data(uint8_t *data_arry, uint16_t write_addr, uint8_t write_num);
void EEPROM_Write_CMD(uint8_t cmd);
uint8_t EEPROM_Read_SR(void);
void EEPROM_Write_SR(uint8_t srdata);
void EEPROM_IO_Init(void);
void Write_Num_TO_EEPROM(uint32_t flash_count);
uint32_t Read_Num_TO_EEPROM(void);

#endif
