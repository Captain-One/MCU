#include "stdio.h"
#include <error_chk.h>
#include "ADC.h"
#include "KEY_FTC332.h"

uint8_t ERROR_Flag_MainFan = 0;
uint8_t ERROR_Flag_ProbeFan = 0;
uint8_t ERROR_Flag_Glass = 0;
uint8_t ERROR_Probe_FAN_Flag = 0;
uint8_t LAMP_Type = 0;
extern uint8_t ERROR_Flag,Charge_TimeOut_Flag,EEPROM_ERROR_Flag,Charge_Over_error;

void MainFan_Error_Chk_Fun(void)
{
  uint16_t MainFan_ReadData;
  MainFan_ReadData = ADC_Read_Channel_Fun(ADC_MAIN_FAN_CHANNEL);
  if((MainFan_ReadData >= MAIN_FAN_SHORT_ERROR_THR) || (MainFan_ReadData <= MAIN_FAN_OPEN_ERROR_THR))
  {
    ERROR_Flag_MainFan = 1;             /* set main fan error flag */
  }else{
    ERROR_Flag_MainFan = 0;
  }
}

void Glass_Error_Chk_Fun(void)
{
  uint16_t Glass_ReadData;
  Glass_ReadData = ADC_Read_Channel_Fun(ADC_CARTRIDG_CHANNEL);
  if( Glass_ReadData <= GLASS_ERROR_THR )
  {
    ERROR_Flag_Glass = 1;             /* set glass error flag */
  }else{
    ERROR_Flag_Glass = 0;
  }
  /*
  if((Glass_ReadData >= GLASS_LAMP_TYPE1_THRL ) || (Glass_ReadData <=  GLASS_LAMP_TYPE1_THRH))
  {
    LAMP_Type = LAMP_1;
  }
  if((Glass_ReadData >= GLASS_LAMP_TYPE2_THRL ) || (Glass_ReadData <=  GLASS_LAMP_TYPE2_THRH))
  {
    LAMP_Type = LAMP_2;
  }
  if( Glass_ReadData >= GLASS_LAMP_TYPE3_THR )
  {
    LAMP_Type = LAMP_3;
  }
  */
}

void ERROR_CHK_Fun(void)
{
  MainFan_Error_Chk_Fun();
  Glass_Error_Chk_Fun();
  if((KEY_TOUCH_PORT->IDR & KEY_TOUCH_PIN))
  {
    ERROR_Probe_FAN_Flag = 0;
  }else{
    ERROR_Probe_FAN_Flag = 1;
  }
  if(0)//EEPROM_ERROR_Flag ||Charge_TimeOut_Flag||Charge_Over_error EEPROM_ERROR_Flag || ERROR_Probe_FAN_Flag)    
  {/*ERROR_Flag_MainFan ERROR_Probe_FAN_Flag EEPROM_ERROR_Flag Charge_TimeOut_Flag ERROR_Flag_Glass*/ 
    ERROR_Flag = 1;
  }else{
    ERROR_Flag = 0;
  }
}