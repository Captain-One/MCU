/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
//#include "stm8s.h"
//#include "LED.h"
#include "GPIO_Config.h"
#include "delay.h"
#include "KEY_FTC332.h"
//#include "UART.h"
#include "stdio.h"
//#include "clock_config.h"
#include "ADC.h"
#include "EEPROM.h"
//#include "error_chk.h"
#include "HV_Charge.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//extern uint8_t LED_Flag;

//extern uint16_t Flash_Count;

void main(void)
{
  //uint32_t e=65536,r;
  Init_Fun();
  while(1)
  {
    //SET_IO_TOG(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
    //SET_IO_TOG(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN);
    //SET_IO_TOG(FULL_CHARGE_PORT,FULL_CHARGE_PIN);
    //SET_IO_TOG(FLASH_TRIG_PORT,FLASH_TRIG_PIN);
    //read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
    //printf("HV is %04x\r\n",read_hv);
    //read_hv = ADC_Read_Channel_Fun(ADC_CARTRIDG_CHANNEL);
    //printf("CAR is %04x\r\n",read_hv);
    //read_hv = ADC_Read_Channel_Fun(ADC_MAIN_FAN_CHANNEL);
    //printf("FAN is %04x\r\n",read_hv);
    Key_Read();
    Key_Process_Fun();
    Function_Processe();
    IWDG->KR = 0xAA;
    //Delay_ms(500);
  } 
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
