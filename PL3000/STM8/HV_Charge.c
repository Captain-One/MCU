#include "HV_Charge.h"
#include "ADC.h"
#include "GPIO_Config.h"
#include "KEY_FTC332.h"
#include "delay.h"
#include "stdio.h"
#include "stm8s.h"
#include "LED.h"

uint8_t Full_Charge_Flag = 0;  /* full charge flag */
extern uint8_t ERROR_Probe_FAN_Flag; /* probe fan or crtridge error flag */
uint8_t Charge_TimeOut_Flag = 0; /* charge timeout flag */
uint8_t Charge_Time_Count = 0, Charge_Over_error = 0; /* charge and discharge timeout count flag */
uint8_t last_hv_level = 0,Discharge_Flag;  /* last hv level and discharge flag */
uint8_t Judgment_Times = 0;
PID HV_PID;

void Charge_Control_Fun(uint8_t hv_level)
{
  uint16_t read_hv;
  switch(hv_level){
  case 1:HV_PID.SetValue = FULL_CHARGE_LEVEL1;break;
  case 2:HV_PID.SetValue = FULL_CHARGE_LEVEL2;break;
  case 3:HV_PID.SetValue = FULL_CHARGE_LEVEL3;break;
  case 4:HV_PID.SetValue = FULL_CHARGE_LEVEL4;break;
  case 5:HV_PID.SetValue = FULL_CHARGE_LEVEL5;break;
  case 6:HV_PID.SetValue = FULL_CHARGE_LEVEL6;break;
  case 7:HV_PID.SetValue = FULL_CHARGE_LEVEL7;break;
  case 8:HV_PID.SetValue = FULL_CHARGE_LEVEL8;break;
  default : HV_PID.SetValue = 0;break;
  }
  read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
  //printf("HV is %04x\r\n",read_hv);
  if((read_hv <= HV_PID.SetValue+3) && (read_hv >= HV_PID.SetValue-3))
  {
    Judgment_Times++; 
    Charge_Time_Count = 0;
    Charge_Over_error = 0;
    if(Judgment_Times == 4)
    {
      Full_Charge_Flag = 1;
      LED_ON(FULL_CHARGE_PORT,FULL_CHARGE_PIN);
      Judgment_Times = 0;
      Charge_Time_Count = 0;
    }
  }else{
      Judgment_Times = 0;
      Full_Charge_Flag = 0;
      LED_OFF(FULL_CHARGE_PORT,FULL_CHARGE_PIN);
      if(read_hv >= HV_PID.SetValue + 50)
      {
        Charge_Over_error = 1;
        SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
        return;
      }
      Charge_Time_Count ++ ;
      if(Charge_Time_Count == 200)
      {
        Charge_TimeOut_Flag = 1;
        SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
        return;
      }
  }  
  if(read_hv <= PID_START_THR)
  {
    SET_IO_LOW(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* ON HV */
  }else{
    PID_Control(&HV_PID,(int16_t)read_hv);
  }
}

void Flash_Control(uint8_t plus_times)
{  
  uint16_t read_hv,Flash_Time_Out = 0;
  TIM1->SR1 = 0x00;
  TIM1->CR1 = 0x80; // disable counter
  SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
  if(plus_times > 6)
  {
    plus_times = 6;
  }
  if(plus_times > 1)
  {
    while(plus_times > 0)
    {
      SET_IO_LOW(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash ON;
      Delay_us(500);
      SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash OFF;
      read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
      while(read_hv >= FLASH_MULTI_OVER_THR)
      {
        Flash_Time_Out++;
        read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
        IWDG->KR = 0xAA;
        if(Flash_Time_Out == FLASH_TIME_OUT)
        {
          //error;
          Flash_Time_Out = 0;
          SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash OFF;
          return;
        }
      }
      SET_IO_LOW(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* ON HV */
      IWDG->KR = 0xAA;
      Delay_ms(250);
      SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
      Flash_Time_Out = 0;
      plus_times--;
    }
  }else{
    SET_IO_LOW(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash ON;
    read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
    while(read_hv >= FLASH_STANDAR_OVER_THR)
    {
      Flash_Time_Out++;
      read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
      IWDG->KR = 0xAA;
      if(Flash_Time_Out == FLASH_TIME_OUT)
      {
        //flash_error;
        //Flash_Time_Out = 0;
        SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash OFF;
        return;
      }
    }
    SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash OFF;
  }
}

int16_t PID_Caculate(PID *pid,int16_t NewValue)
{
  int16_t error,derror;
  error = pid->SetValue - NewValue;
  //pid->SumError += error;
  derror = error - pid->LastError;
  pid->LastError = error;
  //pid->PrevError = pid->LastError;
  return (pid->Kp*error + pid->Kd*derror); //pid->Ki*pid->SumError
}

void PID_Control(PID *pid,int16_t newvalue)
{
  int16_t adjust_value;
  adjust_value = PID_Caculate(pid,newvalue);
  if(adjust_value <= 0)
  {
    SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
  }else{
    SET_IO_LOW(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* ON HV */
    Timer_PID_Control(adjust_value);
  } 
}

void Timer_PID_Control(uint16_t count_data)
{
//  TIM1->EGR |=0x01;           /* Re-initializes counter and prescaler */ 
  TIM1->ARRH = (uint8_t)(count_data >> 8);
  TIM1->ARRL = (uint8_t)count_data;
  TIM1->CR1 |= 0x01; //  start counter
}

void Timer1_Init(void)
{
  disableInterrupts();
  TIM1->EGR |=0x01;           /* Re-initializes counter and prescaler */ 
  TIM1->PSCRH = 0x06; // 16M系统时钟经预分频f=fck/(PSCR+1)
  TIM1->PSCRL = 0x3F; // PSCR=0x1F3F，f=16M/(0x063F+1)=10000Hz，每个计数周期0.1ms
  TIM1->CNTRH = 0;
  TIM1->CNTRL = 0;
 //TIM1->ARRH = 0xFF; // 自动重载寄存器ARR=0xFFFF=65535
 //TIM1->ARRL = 0xFF; // 每记数500次产生一次中断，即6.5535s
  TIM1->IER = 0x01; // 允许更新中断
  TIM1->SR1 = 0x00;
  TIM1->CR1 = 0x80; // disable counter
  enableInterrupts();
}

void HV_PID_Init(void)
{
  HV_PID.Kp = 14;
  HV_PID.Ki = 0;
  HV_PID.Kd = 4;
  HV_PID.LastError = 0;
  HV_PID.SumError = 0;
  Timer1_Init();
}
