#include "LED.h"
#include "KEY_FTC332.h"
#include "GPIO_Config.h"
#include "delay.h"
extern uint8_t Model_Flag;
uint8_t LED_Flash_Long_Count = 0, LED_Flash_Count = 0;

void LED_GPIO_Init(void)
{
  GPIOA->ODR &= ~(LED_LOWLAMP_PIN + LED_ADD_SW_PIN  + LED_MUTE_PIN + LED_LOCK_PIN);  /* OUT 0 ,LED OFF */
  GPIOA->CR2 &= ~(LED_LOWLAMP_PIN + LED_ADD_SW_PIN  + LED_MUTE_PIN + LED_LOCK_PIN);  /* Low Speed 2MHz */
  GPIOA->CR1 |= LED_LOWLAMP_PIN + LED_ADD_SW_PIN  + LED_MUTE_PIN + LED_LOCK_PIN;  /* Push-pull */
  GPIOA->DDR |= LED_LOWLAMP_PIN + LED_ADD_SW_PIN  + LED_MUTE_PIN + LED_LOCK_PIN;  /* Output */
  
  GPIOB->ODR &= ~(LED_LV8_PIN + LED_AUTO_SW_PIN + LED_DEC_SW_PIN);
  GPIOB->CR2 &= ~(LED_LV8_PIN + LED_AUTO_SW_PIN + LED_DEC_SW_PIN);
  GPIOB->CR1 |= LED_LV8_PIN + LED_AUTO_SW_PIN + LED_DEC_SW_PIN;
  GPIOB->DDR |= LED_LV8_PIN + LED_AUTO_SW_PIN + LED_DEC_SW_PIN;
  
  GPIOC->ODR &= ~(LED_POWER_SW_PIN + LED_ERROR_PIN);
  GPIOC->CR2 &= ~(LED_POWER_SW_PIN + LED_ERROR_PIN);
  GPIOC->CR1 |= LED_POWER_SW_PIN + LED_ERROR_PIN;
  GPIOC->DDR |= LED_POWER_SW_PIN + LED_ERROR_PIN;
  
  GPIOD->ODR &= ~(LED_LV5_PIN + LED_LV6_PIN + LED_LV7_PIN);
  GPIOD->CR2 &= ~(LED_LV5_PIN + LED_LV6_PIN + LED_LV7_PIN);
  GPIOD->CR1 |= LED_LV5_PIN + LED_LV6_PIN + LED_LV7_PIN;
  GPIOD->DDR |= LED_LV5_PIN + LED_LV6_PIN + LED_LV7_PIN;
  
  GPIOE->ODR &= ~(LED_LV4_PIN + LED_LV3_PIN+ LED_AUTO_PIN);
  GPIOE->CR2 &= ~(LED_LV4_PIN + LED_LV3_PIN+ LED_AUTO_PIN);
  GPIOE->CR1 |= LED_LV4_PIN + LED_LV3_PIN+ LED_AUTO_PIN;
  GPIOE->DDR |= LED_LV4_PIN + LED_LV3_PIN+ LED_AUTO_PIN;
  
  GPIOG->ODR &= ~(LED_LV2_PIN + LED_LV1_PIN);
  GPIOG->CR2 &= ~(LED_LV2_PIN + LED_LV1_PIN);
  GPIOG->CR1 |= LED_LV2_PIN + LED_LV1_PIN;
  GPIOG->DDR |= LED_LV2_PIN + LED_LV1_PIN;
}

void LED_OFF_ALL(void)
{
  LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
  
  LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
  LED_OFF(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN);
  LED_OFF(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
  LED_OFF(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
  
  LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
  LED_OFF(LED_ERROR_PORT,LED_ERROR_PIN);
  LED_OFF(LED_LOCK_PORT,LED_LOCK_PIN);
  LED_OFF(LED_MUTE_PORT,LED_MUTE_PIN);
  LED_OFF(LED_LOWLAMP_PORT,LED_LOWLAMP_PIN);
  
  LED_OFF(FULL_CHARGE_PORT,FULL_CHARGE_PIN);
  LED_Level(0);
}

void LED_ALL_ON(void)
{
  LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
  LED_ON(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN);
  LED_ON(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
  LED_ON(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
  
  LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
  LED_ON(LED_ERROR_PORT,LED_ERROR_PIN);
  LED_ON(LED_LOCK_PORT,LED_LOCK_PIN);
  LED_ON(LED_MUTE_PORT,LED_MUTE_PIN);
  LED_ON(LED_LOWLAMP_PORT,LED_LOWLAMP_PIN);
  
  LED_ON(FULL_CHARGE_PORT,FULL_CHARGE_PIN);
  
  LED_Level(8);
}

void LED_Level(uint8_t hv_level)
{
  switch(hv_level){
  case 1: LED_ON(LED_LV1_PORT,LED_LV1_PIN);
          LED_OFF(LED_LV2_PORT,LED_LV2_PIN);
          LED_OFF(LED_LV3_PORT,LED_LV3_PIN);
          LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
          LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
          LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
          LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
          LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 2: LED_ON(LED_LV1_PORT,LED_LV1_PIN);
          LED_ON(LED_LV2_PORT,LED_LV2_PIN);
          LED_OFF(LED_LV3_PORT,LED_LV3_PIN);
          LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
          LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
          LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
          LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
          LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 3: LED_ON(LED_LV1_PORT,LED_LV1_PIN);
          LED_ON(LED_LV2_PORT,LED_LV2_PIN);
          LED_ON(LED_LV3_PORT,LED_LV3_PIN);
          LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
          LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
          LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
          LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
          LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 4: LED_ON(LED_LV1_PORT,LED_LV1_PIN);
          LED_ON(LED_LV2_PORT,LED_LV2_PIN);
          LED_ON(LED_LV3_PORT,LED_LV3_PIN);
          LED_ON(LED_LV4_PORT,LED_LV4_PIN);
          LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
          LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
          LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
          LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 5: LED_ON(LED_LV1_PORT,LED_LV1_PIN);
          LED_ON(LED_LV2_PORT,LED_LV2_PIN);
          LED_ON(LED_LV3_PORT,LED_LV3_PIN);
          LED_ON(LED_LV4_PORT,LED_LV4_PIN);
          LED_ON(LED_LV5_PORT,LED_LV5_PIN);
          LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
          LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
          LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 6: LED_ON(LED_LV1_PORT,LED_LV1_PIN);
          LED_ON(LED_LV2_PORT,LED_LV2_PIN);
          LED_ON(LED_LV3_PORT,LED_LV3_PIN);
          LED_ON(LED_LV4_PORT,LED_LV4_PIN);
          LED_ON(LED_LV5_PORT,LED_LV5_PIN);
          LED_ON(LED_LV6_PORT,LED_LV6_PIN);
          LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
          LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 7: LED_ON(LED_LV1_PORT,LED_LV1_PIN);
          LED_ON(LED_LV2_PORT,LED_LV2_PIN);
          LED_ON(LED_LV3_PORT,LED_LV3_PIN);
          LED_ON(LED_LV4_PORT,LED_LV4_PIN);
          LED_ON(LED_LV5_PORT,LED_LV5_PIN);
          LED_ON(LED_LV6_PORT,LED_LV6_PIN);
          LED_ON(LED_LV7_PORT,LED_LV7_PIN);
          LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 8: LED_ON(LED_LV1_PORT,LED_LV1_PIN);
          LED_ON(LED_LV2_PORT,LED_LV2_PIN);
          LED_ON(LED_LV3_PORT,LED_LV3_PIN);
          LED_ON(LED_LV4_PORT,LED_LV4_PIN);
          LED_ON(LED_LV5_PORT,LED_LV5_PIN);
          LED_ON(LED_LV6_PORT,LED_LV6_PIN);
          LED_ON(LED_LV7_PORT,LED_LV7_PIN);
          LED_ON(LED_LV8_PORT,LED_LV8_PIN);
          break;
  default:LED_OFF(LED_LV1_PORT,LED_LV1_PIN);
          LED_OFF(LED_LV2_PORT,LED_LV2_PIN);
          LED_OFF(LED_LV3_PORT,LED_LV3_PIN);
          LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
          LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
          LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
          LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
          LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  }
}

void LED_Multi_Disply(void)
{
  if((Model_Flag == AUTO_MULTI_MODEL) || (Model_Flag == STANDARD_MULTI_MODEL))
  {
    if(LED_Flash_Long_Count < 6)
    {
      LED_Flash_Count++;
      if(LED_Flash_Count == 30)
      {
       LED_Flash_Long_Count++;
       LED_Flash_Count = 0;
       LED_TOG(LED_AUTO_PORT,LED_AUTO_PIN);
      }
    }else{
      LED_Flash_Long_Count = 6;
      LED_Flash_Count++;
      if(LED_Flash_Count == 60)
      {
        LED_Flash_Count = 0;
        LED_Flash_Long_Count = 0;
      }
    }   
  }
}

void LED_Flash_1S(uint8_t lv)
{
  uint8_t i=3;
  LED_Level(0);
  IWDG->KR = 0xAA;
  Delay_ms(800);
  while(i--)
  {
    LED_Level(lv);
    IWDG->KR = 0xAA;
    Delay_ms(800);
    LED_Level(0);
    IWDG->KR = 0xAA;
    Delay_ms(800);
  }
}

void LED_Flash_Times_Disply(uint8_t flash_times)
{
  switch(flash_times){
  case 1: LED_Flash_1S(1);break;
  case 2: LED_Flash_1S(2);break;
  case 3: LED_Flash_1S(3);break;
  case 4: LED_Flash_1S(4);break;
  case 5: LED_Flash_1S(5);break;
  case 6: LED_Flash_1S(6);break;
  case 7: LED_Flash_1S(7);break;
  case 8: LED_Flash_1S(8);break;
  default:break;
  }
}


