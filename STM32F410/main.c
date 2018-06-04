#include <main.h>
#include <stdio.h>
#include "ff.h"
#include "ffconf.h"

#define TxBufferSize1 						(countof(TxBuffer1-1))
#define RxBufferSize1 						(countof(RxBuffer1-1))
#define countof(a)								(sizeof(a)/sizeof(*(a)))
#define BufferSize								(countof(FIleBuffer)-1)

#define FlashWriteAddr			     	0x000000
#define FlashReadAddr				     	FlashWriteAddr
#define FlashSectorEraseAddr 			FlashWriteAddr

BYTE Buffer[100]={0};
uint8_t FIleBuffer[]={"何丁霖，我喜欢你！\r\n"};

FATFS fs;
FIL newfile;
FRESULT res_flash;
UINT fnum;

uint32_t TimeDelya;
uint32_t Flash_ID;

char  SendBuff[500];

void Delay(uint32_t n)
{
	for(;n>0;n--);
}
/*
uint8_t I2C_buff_write[256],I2C_buff_read[256];

uint8_t I2C_Test(void)
{
	uint16_t i;
	printf("write data\n");
	for(i=0;i<255;i++)
	{
		I2C_buff_write[i]=i;
		printf("0x%02X ",I2C_buff_write[i]);
		if(i%16==5)
		{
			printf("\n\r");
		}
	}
	EEPROM_I2C_BytesWrite(I2C_buff_write,0x00,256);
	printf("write OK\n");
	printf("next read\n");
	EEPROM_I2C_BufferRead(I2C_buff_read,0xff,256);
	for(i=0;i<255;i++)
	{
		if(I2C_buff_read[i]!=I2C_buff_write[i])
		{
			printf("0x%02X ",I2C_buff_read[i]);
			printf("data diff\n");
			return 0;
		}
		printf("0x%02X ",I2C_buff_read[i]);
		if(i%16==5)
		{
			printf("\n\r");
		}
	}
	printf("test OK\n");
	return 1;
}

static void ShowMessage(void)
{
	printf("\r\n 这是一个串口控制实 \n");
	printf("使用 USART2 参数: %d 8-N-1 \n",USART_DEBUG_BAUTRATE);
	printf("指令如下: \n");
	printf("1-------------关闭LED \n");
	printf("2-------------打开LED \n");
	printf("3-------------闪烁LED \n");
}
*/
int main(void)
{
	 //KEY_GPIO_Init();
	 //LED_GPIO_Config();
	 DEBUG_USART_Config();
	 printf("This is a Test Of SPI_Falsh FATFS TEST!\r\n");
   res_flash=f_mount(&fs,"3:",1);
	 if(res_flash==FR_NO_FILESYSTEM)
	 {
		 printf("Start geshihua  SPI_Falsh  \r\n");
		 res_flash=f_mkfs("3:",0,0);
		 if(res_flash==FR_OK)
		 {
			 printf("Start geshihua  SPI_Falsh is OK!!! \r\n");
			 res_flash=f_mount(NULL,"3:",1);
			 res_flash=f_mount(&fs,"3:",1);
		 }
		 else
		 {
			 printf("Start geshihua  SPI_Falsh ERROR!!! \r\n");
			 while(1);
		 }
	 }
	 else
	 {
		 if(res_flash!=FR_OK)
		 {
			 printf("Start geshihua  SPI_Falsh LOSER!!! \r\n");
			 printf("Maybe is init error!!! \r\n");
			 while(1);
		 }
		 else
		 {
			 printf("SPI_Falsh FATFS is OK! can test read and write \r\n");
		 }
	 }
	 
	 printf("Start FATFS Write Test!....... \r\n");
	 res_flash=f_open(&newfile,"3:FATFS Read Write Test.txt",FA_CREATE_ALWAYS|FA_WRITE);
	 if(res_flash==FR_OK)
	 {
		 printf("open/mk file secces!,begain write data....... \r\n");
		 res_flash=f_write(&newfile,FIleBuffer,sizeof(FIleBuffer),&fnum);
		 if(res_flash==FR_OK)
		 {
			 printf("write data is OK!num of data is : %d\r\n",fnum);
			 printf("write data is OK!data is : %s\r\n",FIleBuffer);
		 }
		 else
		 {
			 printf("write error! \r\n");
		 }
		 f_close(&newfile);
	 }
	 else{
		 printf("open/mk error! \r\n");
	 }
	 
	 printf("Start FATFS Read Test!....... \r\n");
   res_flash=f_open(&newfile,"3:FATFS Read Write Test.txt",FA_OPEN_EXISTING|FA_READ);
	 if(res_flash==FR_OK)
	 {
		 printf("file open OK!....... \r\n");
		 res_flash=f_read(&newfile,Buffer,sizeof(Buffer),&fnum);
		 if(res_flash==FR_OK)
		 {
			 printf("read data is OK!num of data is : %d\r\n",fnum);
			 printf("read data is OK!data is : %s\r\n",Buffer);
		 }
		 else
		 {
			 printf("read error! %d\r\n",res_flash);
		 }
	 }
	 else
	 {
		 printf("open error! \r\n");
	 }
	 f_close(&newfile);
	 f_mount(NULL,"3:",1);
	 while(1);
}
