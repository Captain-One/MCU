/**
  ******************************************************************************
  * @file    ADC.h
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

#ifndef __HV_CHARGE_H
#define __HV_CHARGE_H

#include <stm8s.h>

#define FLASH_TRIG_PORT                GPIOD
#define FLASH_TRIG_PIN                 GPIO_PIN_0

#define PID_START_THR                  0x02EC   /* 3.656v ----> HV: 677.3V*/
#define FLASH_STANDAR_OVER_THR         0x0006   /* 1.080v ----> HV: 150V*/
#define FLASH_MULTI_OVER_THR           0x02CE   /* 3.512v ----> HV: 650.4V*/         

#define FULL_CHARGE_LEVEL1             0x02F7   /* 3.711v ----> HV: 687.3V*/
#define FULL_CHARGE_LEVEL2             0x030A   /* 3.802v ----> HV: 704.1V*/
#define FULL_CHARGE_LEVEL3             0x031D   /* 3.893v ----> HV: 720.9V*/
#define FULL_CHARGE_LEVEL4             0x032F   /* 3.984v ----> HV: 737.8V*/
#define FULL_CHARGE_LEVEL5             0x0342   /* 4.075v ----> HV: 754.6V*/ 
#define FULL_CHARGE_LEVEL6             0x0354   /* 4.166v ----> HV: 771.5V*/ 
#define FULL_CHARGE_LEVEL7             0x0367   /* 4.257v ----> HV: 788.3V*/ 
#define FULL_CHARGE_LEVEL8             0x037A   /* 4.348v ----> HV: 805.2V*/  

#define FLASH_TIME_OUT                 15000
#define CHARGE_TIME_OUT                45000
#define OPEN_IGBT_TIME_THR             1000                 

typedef struct PID_struct
{
  int16_t SetValue;
  int8_t Kp;
  int8_t Ki;
  int8_t Kd;
  int16_t LastError;
 // int16_t PrevError;
  int16_t SumError;
}
PID;

void Charge_Control_Fun(uint8_t hv_level);
void Flash_Control(uint8_t plus_times);
int16_t PID_Caculate(PID *pid,int16_t NewValue);
void Timer1_Init(void);
void Timer_PID_Control(uint16_t count_data);
void PID_Control(PID *pid,int16_t newvalue);
void HV_PID_Init(void);
    
#endif
