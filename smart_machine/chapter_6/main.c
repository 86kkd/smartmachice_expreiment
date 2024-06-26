#include "main.h"

unsigned long int freq=0;
unsigned long int period=50;// 闸门时间

void display_name (void) {
	unsigned char student_number[] ={1,4} ;
	uint index;
	
	Clear();

	for(index = 0; index < 5; index++)
	{
		write_code_16x16(0x00,(0x01+index)<<4,&chinese[index]);
	}
	
	//display name
	for(index = 0; index < 2; index++)
	{
		write_code_16x16(0x02,(0x01+index+2)<<4,&user_name[index]);
	}
}

unsigned char read_key(void)
{
	unsigned char scan_data = 0x20;//列扫描用IO输出数据
	unsigned char row = 0, col = 0;
	unsigned char key_pos = 0xff;
	//检测是否有按键按下
	XBYTE[ADDR_KEY_WRITE] = 0x00;
	if((XBYTE[ADDR_KEY_READ] & 0x0f) != 0x0f)//有按键按下
	{
		//按键消抖
		Delay5ms();
		
		//进行列检测
		XBYTE[ADDR_KEY_WRITE] = scan_data;
		while((XBYTE[ADDR_KEY_READ] & 0x0f) != 0x0f)//检测到0x0f时即找到该列
		{
			col ++;
			scan_data  = scan_data >> 1;
			XBYTE[ADDR_KEY_WRITE] = scan_data;
		}
		
		//进行行检测
		XBYTE[ADDR_KEY_WRITE] = 0x00;
		switch(XBYTE[ADDR_KEY_READ] & 0x0f)
		{
			case 0x07:row = 0;break;
			case 0x0b:row = 1;break;
			case 0x0d:row = 2;break;
			case 0x0e:row = 3;break;
			default:return 0xff;
			
		}
		//合成按键位置
		key_pos = row*6 + col;
		while((XBYTE[ADDR_KEY_READ] & 0x0f) != 0x0f);
		return key_pos;
	}
	else
		return 0xff;
}

void Init_Timer0(void) {
	TMOD |= 0x01 | 0x00;      //使用模式1，定时器
	TH0=(15536-period)>>8;          	//给定初值
	TL0=(15536-period);         
	EA=1;            //总中断打开
	ET0=1;           //定时器中断打开
	TR0=1;           //定时器开关打开
}

void Init_Timer1(void) {
	TMOD |= 0x10 | 0x40;      //使用模式1，16位计数器
	TH1=0x00;          //给定初值
	TL1=0x00;         
	EA=1;            //总中断打开
	ET1=1;           //定时器中断打开
	TR1=1;           //定时器开关打开
}

void calculate_frequency(){
	// freq = signal_count/period;
	freq = ((TH1<<8)|TL1)/period;
	//给定初值
	TH0=(15536-period)>>8;          	
	TL0=(15536-period); 
	TH1=0;
	TL1=0;
}

void time0_interrupt() interrupt 1 {
	// calculate_frequency();
	gengerage_singal();	
}

void time1_interrrupt() interrupt 3{

}


void main(void) {

	
	uchar key_pos = 0xff;
	uchar tmp_key = 0xff;
	
	//LCD初始化
	Init();
	Init_Timer0();
	Init_Timer1();
	
	display_name();
	
	while(1){
		key_pos = read_key();
		if(key_pos != 0xff){	
			//precess_keyfn(KEY_NUMBER[key_pos]);
			direct_keyfn(KEY_NUMBER[key_pos]);
			tmp_key = key_pos;
		}else {
			key_pos = tmp_key;
			//precess_keyfn(KEY_NUMBER[key_pos]);
			direct_keyfn(KEY_NUMBER[key_pos]);
		}
	}
}