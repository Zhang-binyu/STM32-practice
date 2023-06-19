#include"reg52.h"

typedef unsigned int u16;	//对系统默认数据类型进行重定义
typedef unsigned char u8;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;//设置38译码器的输入端

//定义独立按键控制脚
sbit KEY1=P3^1;
sbit KEY2=P3^0;
sbit KEY3=P3^2;

u8 ms=0;
u8 second=0;
u8 minute=0;
u8 hour=0;		//定义时分秒

u16 num=0;
//设置数码管初值以及中断次数变量

unsigned char code smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
//因为我们的单片机数码管型号是3461AS-1为共阴极数码管

//使用宏定义独立按键按下的键值
#define KEY1_PRESS	1
#define KEY2_PRESS	2
#define KEY3_PRESS	3
#define KEY_UNPRESS	0

#define KEY_MATRIX_PORT P1	//	使用宏定义矩阵按键控制口

void DelayMS(u8 i)
{
	//大约 1ms
	u8 j=100;
	while(i--){
		while(j--);  
	}
}

/*
	函数名		：	key_matrix_ranks_scan
	函数功能	:	使用行列式扫描方法，检测矩阵按键是否按下，按下则返回对应键值
	输入		：	无
	输出		： 	key_value:1-16，对应s1-s16键，
					0:按键未按下
*/

u8 key_matrix_ranks_scan(void)
{
	u8 key_value=0;

	KEY_MATRIX_PORT=0xf7;	//	给第一列赋值0，其余全为1
	if(KEY_MATRIX_PORT!=0xf7)	//	判断第一列按键是否按下
	{
		DelayMS(10);	// 消抖
		switch(KEY_MATRIX_PORT)	//	保存第一列按键按下后的键值
		{
			case 0x77:key_value=1;break;
			case 0xb7:key_value=5;break;
			case 0xd7:key_value=9;break;
			case 0xe7:key_value=13;break;
		}
	}
	while(KEY_MATRIX_PORT!=0xf7);	//	等待按键松开

	KEY_MATRIX_PORT=0xfb;	//	给第二列赋值0，其余全为1
	if(KEY_MATRIX_PORT!=0xfb)	//	判断第二列按键是否按下
	{
		DelayMS(10);	//	消抖
		switch(KEY_MATRIX_PORT)	//	保存第二列按键按下后的键值
		{
			case 0x7b:key_value=2;break;
			case 0xbb:key_value=6;break;
			case 0xdb:key_value=10;break;
			case 0xeb:key_value=14;break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfb);	//	等待按键松开

	KEY_MATRIX_PORT=0xfd;	//	给第三列赋值0，其余全为1
	if(KEY_MATRIX_PORT!=0xfd)	//	判断第三列按键是否按下
	{
		DelayMS(10);	//	消抖
		switch(KEY_MATRIX_PORT)	//	保存第三列按键按下后的键值
		{
			case 0x7d:key_value=3;break;
			case 0xbd:key_value=7;break;
			case 0xdd:key_value=11;break;
			case 0xed:key_value=15;break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfd);	//等待按键松开

	KEY_MATRIX_PORT=0xfe;	//给第四列赋值0，其余全为1
	if(KEY_MATRIX_PORT!=0xfe)	//判断第四列按键是否按下
	{
		DelayMS(10);	//消抖
		switch(KEY_MATRIX_PORT)	//保存第四列按键按下后的键值
		{
			case 0x7e:key_value=4;break;
			case 0xbe:key_value=8;break;
			case 0xde:key_value=12;break;
			case 0xee:key_value=16;break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfe);	//等待按键松开

	return key_value;
}

void DIGdisplay()
{	 	
	 	 LSA = 0 ;LSB = 0 ; LSC = 0 ;P0 = smgduan[second%10];DelayMS(1);P0=0x00;  	//第一位数码管，消影		
		 LSA = 1 ;LSB = 0 ; LSC = 0 ;P0 = smgduan[second/10];DelayMS(1);P0=0x00; 		//第二位数码管，消影
		 

		 LSA = 1 ;LSB = 1 ; LSC = 0 ;P0 = smgduan[minute%10];DelayMS(1);P0=0x00;		//第四位数码管，消影
		 LSA = 0 ;LSB = 0 ; LSC = 1 ;P0 = smgduan[minute/10];DelayMS(1);P0=0x00;		//第五位数码管，消影
		 
		 LSA = 0 ;LSB = 1 ; LSC = 1 ;P0 = smgduan[hour%10];DelayMS(1);P0=0x00;		//第六位数码管，消影
		 LSA = 1 ;LSB = 1 ; LSC = 1 ;P0 = smgduan[hour/10];DelayMS(1);P0=0x00;		//第七位数码管，消影
		 
		 LSA = 0 ;LSB = 1 ; LSC = 0 ;P0 = 0x40;DelayMS(1);P0=0x00;			
		 LSA = 1 ;LSB = 0 ; LSC = 1 ;P0 = 0x40;DelayMS(1);P0=0x00;		//第五位和第三位显示一杠，消影													 
}	 


void time0_init(void)
{
	TMOD=0X01;//选择为定时器0模式，工作方式1
	TH0=0xFC;	//给定时器赋初值，定时50ms
	TL0=0x66;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	
}

void time0()interrupt 1//定时器0中断函数
{
	TH0 = 0xFC;		//设置定时初值  1ms
	TL0 = 0x66;		//设置定时初值

	num++;
	if(num==1000)        //1ms
	{
   		num=0;
    	ms++;
		if(ms==1000){
			ms=0;
			second++;
		}
		if(second==60){
			second=0;
			minute++;
		}
		if(minute==60){
			minute=0;
			hour++;
		}
		if(hour==24){
			hour=0;
		}
	}						
}
void main()
{
	u8 key=0;
	time0_init();
	while(1){
		DIGdisplay();
		if(KEY1==0){
			DelayMS(1);
			if(KEY1==0){
				TR0=1;
				while(KEY1==0);
			}
		}if(KEY2==0){
			DelayMS(1);
			if(KEY2==0){
				TR0=0;
				while(KEY2==0);
			}
		}if(KEY3==0){
			DelayMS(1);
			if(KEY3==0){
				hour=0;
				minute=0;
				second=0;
				while(KEY3==0);
			}
		}
		key=key_matrix_ranks_scan();
		if(key!=0){
			switch(key){
				case 1:if(second!=59)second++;break;		//秒增一
				case 2:if(second!=0)second--;break;			//秒减一
				case 3:second=59;break;						//秒置为五十九
				case 4:second=0;break;						//秒置为零
			
				case 5:if(minute!=59)minute++;break;		//分钟增一
				case 6:if(minute!=0)minute--;break;			//分钟减一
				case 7:minute=59;break;						//分钟置为五十九
				case 8:minute=0;break;						//分钟置为零
			
				case 9:if(hour!=23)hour++;break;			//小时增一
				case 10:if(hour!=0)hour--;break;			//小时减一
				case 11:hour=23;break;						//小时置为五十九
				case 12:hour=0;break;						//小时置为零
			}
		}
	}
}
