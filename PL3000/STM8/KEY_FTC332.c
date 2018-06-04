/**
  ******************************************************************************
  * @file    KEY_FTC332.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define LED Pin, PORT and Functions.
   ******************************************************************************
  * @attention
  * this file is FTC332 driver and Key function
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#include "KEY_FTC332.h"
#include "UART.h"
#include "stdio.h"
#include "GPIO_Config.h"
#include "LED.h"
#include "HV_Charge.h"
#include "delay.h"
#include "error_chk.h"
#include "EEPROM.h"

uint8_t Key_Value_Error = 0;  /* Key read data error flag */
uint8_t Start_Flag = NO;  /* FTC332 data Start Flag */
uint8_t Key_Read_OK = 0; /* Read One Fram Key_Value OK flag */
uint8_t Bit_Time_H, Bit_Time_L; /* one key value bit timer count data */
uint8_t Bit_Count = 0;  /* count read bit num */

uint8_t Key_Int = 0;  /* Key into Interrupt flag */
uint8_t Key_Trg = 0,Key_Cont = 0, Key_count = 0;       /* key read Function var */
uint8_t Flash_Key_Cont = 0,Flash_Key_Tirg = 0;
uint8_t Key_Byte_Value = 0x00; /* key value to a byte */

uint8_t Power_Flag = OFF;       /* POWER Flag, defualt OFF */
uint8_t Mute_Flag = OFF;        /* MUTE Flag, defualt OFF */
//uint8_t UP_Flag = OFF;          /* Key UP Flag, defualt OFF */
//uint8_t Down_Flag = OFF;        /* KEY Down Flag, defualt OFF */
uint8_t Lock_Flag = ON;         /* LOCK Flag, defualt ON */
uint8_t Model_Flag = STANDARD_MODEL;   /* Model Flag, defualt Standard model */
uint8_t Multi_Flag = 0;
uint8_t Flash_Key_Push = 0;
//uint8_t Flash_Touch_Push = 0;
uint8_t Flash_Key_Count = 0;
uint8_t Flash_Delay_Count = 0;
//uint8_t Flash_Temp_Count = 0;
uint8_t First_Flash = 1;
uint8_t Flash_Trig = 0;
uint16_t time_out_power_off = 0;
uint8_t Fisrt_Push = 1;

uint8_t HV_Level = 0;         /* HV Level count */
uint32_t Flash_Count ;  /* flash times */
uint8_t Lowlamp_Flag = 0; /* lowlam flag */

uint8_t ERROR_Flag = 0; /* all error flag */
extern uint8_t ERROR_Probe_FAN_Flag,Full_Charge_Flag; /* probe fan error flag full and charge flag */
uint8_t Key_Push = 0; /* key push flag it is use to long push judgment */

uint16_t Read_HV;  /* read HV vlaue */

void Key_Init_Fun(void)
{
  disableInterrupts();
  KEY_FLASH_PORT->ODR |= KEY_FLASH_PIN;
  KEY_FLASH_PORT->DDR &=~KEY_FLASH_PIN;
  KEY_FLASH_PORT->CR1 |=  KEY_FLASH_PIN;
  KEY_FLASH_PORT->CR2 &= ~KEY_FLASH_PIN;
  
  KEY_TOUCH_PORT->ODR |= KEY_TOUCH_PIN;
  KEY_TOUCH_PORT->DDR &=~KEY_TOUCH_PIN;
  KEY_TOUCH_PORT->CR1 |=  KEY_TOUCH_PIN;
  KEY_TOUCH_PORT->CR2 &= ~KEY_TOUCH_PIN;;
  
  KEY_PORT->DDR &= ~KEY_PIN;//PB4 input
  KEY_PORT->CR1 |=  KEY_PIN;  //pull up
  KEY_PORT->CR2 &= ~KEY_PIN; //disable externnal interrupt  
  
 // ITC->ISPR2 &= (3<<0);         /* clean priority */
 // ITC->ISPR2 |= (1<<0);         /* set PB Port priority 1 */
  
  EXTI->CR1 = 0x04;                /* set PB falling edge */
  Key_Timer_Init_Fun();
  enableInterrupts();
}

void Key_Timer_Init_Fun(void)
{
  KEY_TIMER->IER = 0x00;           /* disable all interrupt */
  KEY_TIMER->CNTRH = 0x00;
  KEY_TIMER->CNTRL = 0x00; 
  KEY_TIMER->PSCR = 0x04;          /* set prescaler : 4 Fck_cnt = 16MHz/32 = 0.5MHz */
  KEY_TIMER->EGR |=0x01;           /* Re-initializes counter and prescaler */      
  KEY_TIMER->CR1 = 0x08;           /* set auto reload, disable counter */
}

void Key_Read(void)
{
  uint8_t t = 0x01;
  uint16_t timeout = KEY_READ_TIME_OUT;
  uint16_t Bit_Time = 0;  /* 16 bit timer count data */
  uint8_t Flash_Key_Readdata; 
  KEY_PORT->CR2 |= KEY_PIN; //enable externnal interrupt
  while(!Key_Read_OK)
  { 
   timeout--;
   if(timeout <= 0)
    {
      KEY_PORT->CR2 &= ~KEY_PIN; //disable externnal interrupt 
      Key_Read_OK = 0;
      break;
    }
    if(Key_Int == 1)
    {
      Key_Int  = 0;    
      Bit_Time = Bit_Time_H;
      Bit_Time = (Bit_Time << 8) | Bit_Time_L;     /* read count data */       
      if(!Start_Flag)
      {
        if((Bit_Time > KEY_START_BIT_THRL) && (Bit_Time < KEY_START_BIT_THRH))
        { 
          Start_Flag = YES;
        }
      }else{
        if(Bit_Count < 8)
        {
          if((Bit_Time > KEY_H_BIT_THRL) && (Bit_Time < KEY_H_BIT_THRH))
          {
            Key_Byte_Value |= t;
          }else{
            if((Bit_Time > KEY_L_BIT_THRL) && (Bit_Time < KEY_L_BIT_THRH))
            {
              Key_Byte_Value &=~t;
            }else{
              Key_Byte_Value &=~t;   /* if read data error, set key not push */
            }
          }
          t = t << 1; 
          Bit_Count ++;
        }else{
          Bit_Count = 0;
          Key_Read_OK = 1;
          Start_Flag = NO;
          KEY_TIMER->CNTRH = 0;
          KEY_TIMER->CNTRL = 0;
          KEY_TIMER->CR1 = 0x08;           /* disable counter, stop count */
        }
      }
    }
  }
  KEY_PORT->CR2 &= ~KEY_PIN; //disable externnal interrupt 
  Key_Read_OK = 0;
  Key_Analysis();
  if(Lock_Flag == OFF)
  {
    Flash_Key_Readdata = KEY_FLASH_PORT->IDR ^ 0xff;
    Flash_Key_Tirg = Flash_Key_Readdata & (Flash_Key_Readdata ^ Flash_Key_Cont);
    Flash_Key_Cont = Flash_Key_Readdata;
  }
}

void Key_Analysis(void)
{
  uint8_t h;
  h = (Key_Byte_Value & 0xf0) >> 4;
  if(h != (~Key_Byte_Value & 0x0f))
  {
    Key_Value_Error = 1;
  }else{
    Key_Value_Error = 0;
    Key_Byte_Value = Key_Byte_Value & 0x0f;
    Key_Trg = Key_Byte_Value & (Key_Byte_Value ^ Key_Cont);
    Key_Cont = Key_Byte_Value;
  }
}

void Key_Process_Fun(void)
{
  if(Power_Flag == ON)
  {
    if(Key_Cont == KEY_POWER)
    {
      if(Key_Trg == KEY_POWER)
      { 
       Key_count = 0;
       Key_Push = KEY_POWER;
      }else{
       if(Key_Push == KEY_POWER)
       {
         Key_Power_Off_Proc();
       }
      }
    }
    if(Lock_Flag == ON)
    {
      if(Key_Cont == KEY_UNLOCK)
      {
         if((Key_Trg == KEY_ADD) ||(Key_Trg == KEY_DEC) || (Key_Trg == KEY_UNLOCK))
         {
           Key_count = 0;
           Key_Push = KEY_UNLOCK;
           LED_ON(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
           LED_ON(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
         }else{
           if(Key_Push == KEY_UNLOCK)
           {
             Key_Unlock_Proc();
           }
         }
      }
      if(Key_Push != KEY_POWER)
      {
        if(!(Key_Cont & KEY_UNLOCK) || !(Key_Cont & KEY_ADD) || !(Key_Cont & KEY_DEC))
        {
          Key_Push = 0;
          LED_OFF(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
          LED_OFF(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
        }
      }
    }else{
      switch(Key_Cont)
      {
        case KEY_AUTO   :  if(Key_Trg == KEY_AUTO)
                           { 
                             Key_count = 0;
                             Key_Push = KEY_AUTO;
                             if(Mute_Flag == OFF)
                             {
                               BEEP_ON(BEEP_PORT,BEEP_PIN);
                             }                                  
                             LED_ON(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN);
                           }else{                                  
                             if(Key_Push == KEY_AUTO)
                             {
                               Key_Mute_Proc();
                             }
                             BEEP_OFF(BEEP_PORT,BEEP_PIN);
                           } break;
        case KEY_ADD    :  if(Key_Trg == KEY_ADD)
                           {
                             Key_count = 0;
                             if(Mute_Flag == OFF)
                             {
                               BEEP_ON(BEEP_PORT,BEEP_PIN);
                             }   
                             LED_ON(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
                             Key_Level_Up_Proc();
                           }else{
                             BEEP_OFF(BEEP_PORT,BEEP_PIN);                                                                    
                           }break;
        case KEY_DEC    :  if(Key_Trg == KEY_DEC)
                           {
                             Key_count = 0;
                             if(Mute_Flag == OFF)
                             {
                               BEEP_ON(BEEP_PORT,BEEP_PIN);
                             }   
                             LED_ON(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
                             Key_Level_Down_Proc(); 
                           }else{
                             BEEP_OFF(BEEP_PORT,BEEP_PIN);                               
                           }break;
        default : //Key_count = 0;
                  BEEP_OFF(BEEP_PORT,BEEP_PIN);
                  break;
      }
      if(!(Key_Cont & KEY_AUTO))
      {
        LED_OFF(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN);
        if(Key_Push == KEY_AUTO)
        {
          Key_Push = 0;
          if(Key_count <= 50)
          {
            if(Lock_Flag == OFF)
            {
              if(Model_Flag != AUTO_MODEL)
              {
                Model_Flag = AUTO_MODEL;
                //Flash_Temp_Count = 0;
                Flash_Delay_Count = 0;
                LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
              }else{
                Model_Flag = STANDARD_MODEL;
                LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
              }
            }
          }      
        }
      }
      if(!(Key_Cont & KEY_ADD))
      {
        LED_OFF(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
      }
      if(!(Key_Cont & KEY_DEC))
      {
        LED_OFF(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
      }
      if((Flash_Key_Cont & KEY_TOUCH_PIN) && (!Flash_Key_Push))
      {
        if(Flash_Key_Cont & KEY_FLASH_PIN)
        {
          if(Flash_Key_Tirg & KEY_FLASH_PIN)
          {
            Flash_Key_Push = 1;
            Flash_Trig = 1;
          }
        }
      }else{
        if(Flash_Key_Cont & KEY_FLASH_PIN)
        {
          if(Flash_Key_Tirg & KEY_FLASH_PIN)
          {
            Flash_Key_Push = 1;
          }
          if(Flash_Key_Push)
          {
            Flash_Key_Count++;
            if(Flash_Key_Count >= 80)
            {
              Flash_Trig = 0;
              Flash_Key_Count = 0;
              Flash_Key_Push = 0;              
              switch(Model_Flag){
              case AUTO_MODEL : Model_Flag = AUTO_MULTI_MODEL;
                                HV_Level = 1;
                                Flash_Delay_Count = 0;break;
              case STANDARD_MODEL : Model_Flag = STANDARD_MULTI_MODEL;
                                    HV_Level = 1;
                                    Flash_Delay_Count = 0;break;
              case AUTO_MULTI_MODEL : Model_Flag = AUTO_MODEL;
                                      //Flash_Temp_Count = 0;
                                      Flash_Delay_Count = 0;
                                      LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);break;
              case STANDARD_MULTI_MODEL : Model_Flag = STANDARD_MODEL;
                                          Flash_Delay_Count = 0;
                                          LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);break;
              default:break;               
              }
              if(Mute_Flag == OFF)
              {
                BEEP_ON(BEEP_PORT,BEEP_PIN);
                Delay_ms(200);
                BEEP_OFF(BEEP_PORT,BEEP_PIN);
              }
            }
          }
      }else{
        Flash_Key_Count = 0;
        Flash_Key_Push = 0;
      }
     }
    }
    if((Key_Cont == 0) && (!(Flash_Key_Cont & KEY_TOUCH_PIN)) && (!(Flash_Key_Cont & KEY_FLASH_PIN)))
    {
      time_out_power_off++;
      if(time_out_power_off == TIME_OUT_10M)
      {
        time_out_power_off = 0;
        Key_count = 49;
        Key_Power_Off_Proc();
      }
    }else{
      time_out_power_off = 0;
    }
  }else{
    if(Key_Cont == KEY_POWER)
    {
     if(Key_Trg == KEY_POWER)
     {
       Key_count = 0;
       Key_Push = KEY_POWER;
     }else{
       if(Key_Push == KEY_POWER)
       {
        Key_Power_On_Proc();            
       }                    
     }
    }
  }
}

void Key_Power_On_Proc(void)
{
  Key_count ++;
  if(Key_count == 50)
  {
    LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);      /* power LED ON*/
    BEEP_ON(BEEP_PORT,BEEP_PIN);
    LED_ALL_ON();
    IWDG->KR = 0xAA;
    Delay_ms(200);
    IWDG->KR = 0xAA;
    BEEP_OFF(BEEP_PORT,BEEP_PIN);
    Delay_ms(900);
    IWDG->KR = 0xAA;
    LED_OFF_ALL();
    LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);      /* power LED ON*/
    LED_ON(LED_LOCK_PORT,LED_LOCK_PIN);         /* Lock LED ON*/
   // LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);         /* Lock LED ON*/  
    SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);      /* OFF HV */
    SET_IO_HIGH(PROBE_POWER_PORT,PROBE_POWER_PIN);      /* open probe power */
    Power_Flag = ON;
    First_Flash = 1;
    Flash_Count = Read_Num_TO_EEPROM();
   // printf("FAN is %ld\r\n",Flash_Count);
    if(Flash_Count <= 10000)
    {
      Lowlamp_Flag = 0;
      LED_Flash_Times_Disply(1);
    }else{
      if(Flash_Count <= 20000)
      {
        Lowlamp_Flag = 0;
        LED_Flash_Times_Disply(2);
      }else{
        if(Flash_Count <= 30000)
        {
          Lowlamp_Flag = 0;
          LED_Flash_Times_Disply(3);
        }else{
          if(Flash_Count <= 40000)
          {
            Lowlamp_Flag = 0;
            LED_Flash_Times_Disply(4);
          }else{
            if(Flash_Count <= 50000)
            {
              Lowlamp_Flag = 0;
              LED_Flash_Times_Disply(5);
            }else{
              if(Flash_Count <= 60000)
              {
                Lowlamp_Flag = 0;
                LED_Flash_Times_Disply(6);
              }else{
                if(Flash_Count <= 70000)
                {
                  Lowlamp_Flag = 0;
                  LED_Flash_Times_Disply(7);
                }else{
                  if(Flash_Count <= 80000)
                  {
                    Lowlamp_Flag = 0;
                    LED_Flash_Times_Disply(8);
                  }else{
                    Lowlamp_Flag = 1;
                  }
                }
              }
            }
          }
        }
      }
    }
    Key_count = 0;
    Key_Push = 0;
  }
}

void Key_Power_Off_Proc(void)
{
  Key_count ++;
  if(Key_count == 50)
  {
    Power_Flag = OFF;    
    SET_IO_LOW(PROBE_POWER_PORT,PROBE_POWER_PIN);      /* Off probe power */
    SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);      /* OFF HV */
    LED_OFF(FULL_CHARGE_PORT,FULL_CHARGE_PIN);
    //init all var;
    Full_Charge_Flag = 0;
    Power_Flag = OFF;       /* POWER Flag, defualt OFF */
    Mute_Flag = OFF;        /* MUTE Flag, defualt OFF */
   // UP_Flag = OFF;          /* Key UP Flag, defualt OFF */
   // Down_Flag = OFF;        /* KEY Down Flag, defualt OFF */
    Model_Flag = STANDARD_MODEL;  /* AUTO Flag, defualt Standard model */
    Lock_Flag = ON;         /* LOCK Flag, defualt ON */
    ERROR_Flag = 0;
    HV_Level = 0;
    Key_Push = 0;
    Key_count = 0;
    BEEP_ON(BEEP_PORT,BEEP_PIN);
    Delay_ms(200);
    BEEP_OFF(BEEP_PORT,BEEP_PIN);
    LED_OFF_ALL();
  }
}

void Key_Level_Up_Proc(void)
{
  HV_Level ++;
  if(HV_Level == 8)
  {
    HV_Level = 8;
  }      
  if(First_Flash || (Model_Flag == AUTO_MODEL))
  {
    if(HV_Level > 5)
    {
      HV_Level = 5;
      //beep two;
    }        
  }
  if((Model_Flag == STANDARD_MULTI_MODEL) || (Model_Flag == AUTO_MULTI_MODEL))
  {
    HV_Level = 1;
  }
}

void Key_Level_Down_Proc(void)
{
  if(HV_Level == 1)
  {
    HV_Level = 1;
  }else{
    HV_Level --;
  }
}

void Key_Mute_Proc(void)
{
  Key_count++;
  if(Key_count >= 80)
  {
    Key_Push = 0;
    if(Mute_Flag == ON)
    {
      Mute_Flag = OFF;
      LED_OFF(LED_MUTE_PORT,LED_MUTE_PIN);
      BEEP_ON(BEEP_PORT,BEEP_PIN); 
      Delay_ms(20);
    }else{
      Mute_Flag = ON;
      LED_ON(LED_MUTE_PORT,LED_MUTE_PIN);
    }    
  }
}

void Key_Unlock_Proc(void)
{
  if((Lock_Flag == ON) && (!ERROR_Flag))
  {
    Key_count ++;
    if(Key_count == 50)
    {
      Lock_Flag = OFF;
      Key_Push = 0;
      BEEP_ON(BEEP_PORT,BEEP_PIN);
      Delay_ms(100);
      BEEP_OFF(BEEP_PORT,BEEP_PIN);
      LED_OFF(LED_LOCK_PORT,LED_LOCK_PIN);
      HV_Level = 1;    
      Key_count = 0;
    }
  }
}

void Function_Processe(void)
{
  if(Power_Flag == ON)
  {
    ERROR_CHK_Fun();    /* error check, set error flag*/
    LED_Multi_Disply();
    LED_Level(HV_Level);    
  //  printf("Lowlamp_Flag %d\r\n",Lowlamp_Flag);
    if((!ERROR_Flag) && (!Lowlamp_Flag))
    {
      LED_OFF(LED_ERROR_PORT,LED_ERROR_PIN);
      if(Lock_Flag == OFF)
      {
        Charge_Control_Fun(HV_Level);
        if(Full_Charge_Flag)
        {
          switch(Model_Flag){
          case AUTO_MODEL : Auto_Model_Flash();break;
          case STANDARD_MODEL : Standard_Model_Flash();break;
          case AUTO_MULTI_MODEL : Multi_Auto_Modle_Flash();break;
          case STANDARD_MULTI_MODEL : Multi_Standard_Modle_Flash();break;
          default : break;
          }
        }
      }
    //  LED_Level(HV_Level);
      if(Flash_Count >= 80000)
      {
        Lowlamp_Flag = 1;
      }else{
        Lowlamp_Flag = 0;
      }
    }else{
      if(ERROR_Flag)
      {
        LED_ON(LED_ERROR_PORT,LED_ERROR_PIN);
        Lock_Flag = ON;
      }
      if(Lowlamp_Flag)
      {
        LED_ON(LED_LOWLAMP_PORT,LED_LOWLAMP_PIN);
        Lock_Flag = ON;
      }
    }    
  }
}

void Auto_Model_Flash(void)
{
  uint8_t delay_count;
  if(!(KEY_TOUCH_PORT->IDR & KEY_TOUCH_PIN))
  {
    //delay;
    if(Fisrt_Push)
    {
      delay_count = 50;
      Fisrt_Push = 0;
    }else{
      delay_count = 100;
    }
    Flash_Delay_Count++;
    if(Flash_Delay_Count == delay_count)
    {
      //Flash_Touch_Push = 1;
      Flash_Delay_Count = 0;
      //Flash_Temp_Count++;
      //if(Flash_Temp_Count <= 6)
      //{
        Flash_Control(1);  //flash
        First_Flash = 0;
        SET_IO_LOW(FULL_CHARGE_PORT,FULL_CHARGE_PIN);           
        Full_Charge_Flag = 0;
        if(HV_Level <= 2)
        {
          Flash_Count++;
        }else{
          if(HV_Level <= 5)
          {
            Flash_Count += 3;
          }else{
            Flash_Count += 5;
          }
        }        
        Write_Num_TO_EEPROM(Flash_Count);//write data to eeprom, disable write;
      //}else{
       // Flash_Temp_Count = 6;
       // Flash_Touch_Push = 0;
       //}
    }
  }else{
  //  Flash_Touch_Push = 0;
    Flash_Delay_Count = 0;
    //Flash_Temp_Count = 0;
    Fisrt_Push = 1;
  }
}

void Standard_Model_Flash(void)
{
  if(Flash_Trig)
  {
    Flash_Trig = 0;
    Flash_Control(1); 
    First_Flash = 0;
    SET_IO_LOW(FULL_CHARGE_PORT,FULL_CHARGE_PIN);            
    Full_Charge_Flag = 0;
    if(HV_Level <= 2)
    {
      Flash_Count++;
    }else{
      if(HV_Level <= 5)
      {
        Flash_Count += 3;
      }else{
        Flash_Count += 5;
      }
    }
    Write_Num_TO_EEPROM(Flash_Count);//write data to eeprom, disable write;
  }
}

void Multi_Auto_Modle_Flash(void)
{
  uint8_t delay_count;
  if(!(KEY_TOUCH_PORT->IDR & KEY_TOUCH_PIN))
  {
    if(Fisrt_Push)
    {
      delay_count = 50;
      Fisrt_Push = 0;
    }else{
      delay_count = 100;
    }
    Flash_Delay_Count++;
    if(Flash_Delay_Count == delay_count)
    {
      Flash_Trig = 0;
      Flash_Delay_Count = 0;
      Flash_Control(4);
      First_Flash = 0;   
      SET_IO_LOW(FULL_CHARGE_PORT,FULL_CHARGE_PIN);            
      Full_Charge_Flag = 0;
      if(HV_Level <= 2)
      {
        Flash_Count++;
      }else{
        if(HV_Level <= 5)
        {
          Flash_Count += 3;
        }else{
          Flash_Count += 5;
        }
      }
      Write_Num_TO_EEPROM(Flash_Count);//write data to eeprom, disable write;   
    }
  }else{
    Fisrt_Push = 1;
    Flash_Delay_Count = 0;
  }
}

void Multi_Standard_Modle_Flash(void)
{
  if(Flash_Trig)
  {
    Flash_Trig = 0;
    Flash_Delay_Count = 0;
    Flash_Control(4);
    First_Flash = 0;   
    SET_IO_LOW(FULL_CHARGE_PORT,FULL_CHARGE_PIN);            
    Full_Charge_Flag = 0;
    if(HV_Level <= 2)
    {
      Flash_Count++;
    }else{
      if(HV_Level <= 5)
      {
        Flash_Count += 3;
      }else{
        Flash_Count += 5;
      }
    }
    Write_Num_TO_EEPROM(Flash_Count);//write data to eeprom, disable write;   
  }
}
