#include <STC15W.h>
#define ulong unsigned long
#define uchar unsigned char     
#define uint unsigned int 
sbit LED1 =P3^7;
sbit LED2 =P3^6;
sbit LED3 =P3^3;
sbit BUZZ =P3^2;
sbit KEY1 =P1^1;
sbit KEY2 =P1^2;
sbit KEY3 =P1^3;
sbit KEY4 =P1^4;
char KEY1_T,KEY2_T,KEY3_T,n; 

uchar timer_count = 0;
uchar cont = 0;
uchar trg = 0;
uchar led1_on = 0;
uchar led2_on = 0;
uchar led3_on = 0;
uchar beep_on = 0;


void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay10ms()		//@11.0592MHz
{
	unsigned char a,b ;

	a = 108;
	b = 145;
	do
	{
		while (--b);
	} while (--a);
}

void led1ss()
{
	if(timer_count == 50)
	{
		LED1=~LED1;
		timer_count = 0;
	}
}
void led2ss()
{
	if(timer_count == 50)
	{
		LED2=~LED2;
		timer_count = 0;
	}
}

void Key_scan()
{
	uchar temp;
	uchar read;
	temp = P1 | 0xe1; //
	read  = temp ^ 0xff;
	trg = read & (read ^ cont);
	cont = read;
	
	switch(cont)
	{
		case 0x02: if(trg & 0x02) //key1 按下
				   {
					  timer_count = 0;
					  led1_on ^= 1;
					  led2_on = 0;
				   }break;
		case 0x04: if(trg & 0x04)
				   {
					  timer_count = 0;
					  led2_on ^= 1;
					  led1_on = 0;
				   }break;
		case 0x08: if(trg & 0x08)
				   {
					  led3_on ^= 1;
				   }break;
		case 0x10: beep_on = 1;break;
	    default:break;
	}
	if((cont & 0x10) == 0) //key4 释放
	{
		beep_on = 0;
	}
}

void LED_dis()
{
	if(led1_on)
	{
		led1ss();
	}else{
		LED1 = 0;
	}
	if(led2_on)
	{
		led2ss();
	}else{
		LED2 = 0;
	}
	if(led3_on)
	{
		LED3 = 1;
	}else{
		LED3 = 0;
	}
}

void Beep_do()
{
	if(beep_on)
	{
		BUZZ = 1;
	}else{
		BUZZ = 0;
	}
}


void main()
{	 
	LED1=0;
    LED2=0;
    LED3=0;
    BUZZ=0;
	
	while(1)
	{
		Key_scan();
		LED_dis();
		Beep_do();
		Delay10ms();
		timer_count++;
	}
}
