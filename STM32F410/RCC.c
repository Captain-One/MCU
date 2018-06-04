#include "RCC.h"

void HSE_USER_SetSysClock(uint32_t m,uint32_t n,uint32_t p,uint32_t q,uint32_t r)
{
	 __IO uint32_t HSEStartUpStatus = 0;
	
	 RCC_HSEConfig(RCC_HSE_ON);
	 HSEStartUpStatus = RCC_WaitForHSEStartUp();
	 if(HSEStartUpStatus==SUCCESS)
	 {
		 RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		 PWR->CR |= PWR_CR_VOS;
		 RCC_HCLKConfig(RCC_SYSCLK_Div1);
		 RCC_PCLK2Config(RCC_HCLK_Div2);
		 RCC_PCLK1Config(RCC_HCLK_Div4);
		 RCC_PLLConfig(RCC_PLLSource_HSE,m,n,p,q,r);
		 RCC_PLLCmd(ENABLE);
		 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET){
		 }
		 PWR->CR |= PWR_CR_ODEN;
		 while((PWR->CSR & PWR_CSR_ODRDY)==0){
		 }
		 PWR->CR |= PWR_CR_ODSWEN;
		 while((PWR->CSR & PWR_CSR_ODSWRDY)==0){
		 }
		 FLASH->ACR = FLASH_ACR_PRFTEN 
		              | FLASH_ACR_ICEN 
		              | FLASH_ACR_DCEN 
		              | FLASH_ACR_LATENCY_5WS;
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
     while(RCC_GetSYSCLKSource()!=0x08){
		 }			 
	 }else{
		 while(1){
		 }
	 }
}

void HSI_USER_SetSysClock(uint32_t m,uint32_t n,uint32_t p,uint32_t q,uint32_t r)
{
	__IO uint32_t HSIStartUpStatus = 0;
	
	 RCC_DeInit();
	 RCC_HSICmd(ENABLE);
	 HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;
	 if(HSIStartUpStatus==RCC_CR_HSIRDY)
	 {
		 RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		 PWR->CR |= PWR_CR_VOS;
		 RCC_HCLKConfig(RCC_SYSCLK_Div1);
		 RCC_PCLK2Config(RCC_HCLK_Div2);
		 RCC_PCLK1Config(RCC_HCLK_Div4);
		 RCC_PLLConfig(RCC_PLLSource_HSI,m,n,p,q,r);
		 RCC_PLLCmd(ENABLE);
		 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET){
		 }
		 PWR->CR |= PWR_CR_ODEN;
		 while((PWR->CSR & PWR_CSR_ODRDY)==0){
		 }
		 PWR->CR |= PWR_CR_ODSWEN;
		 while((PWR->CSR & PWR_CSR_ODSWRDY)==0){
		 }
		 FLASH->ACR = FLASH_ACR_PRFTEN 
		              | FLASH_ACR_ICEN 
		              | FLASH_ACR_DCEN 
		              | FLASH_ACR_LATENCY_5WS;
		 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		 while(RCC_GetSYSCLKSource()!=0x08){
		 }			 
	 }else{
		 while(1){
		 }
	 }
}

void Delay(__IO uint32_t n)
{
	for(;n>0;n--);
}

void MCO1_CPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(MCO1_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =MCO1_PIN; 
	GPIO_Init(MCO1_PORT,&GPIO_InitStructure);
}

void MCO2_CPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(MCO2_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin =MCO2_PIN; 
	GPIO_Init(MCO2_PORT,&GPIO_InitStructure);
}


