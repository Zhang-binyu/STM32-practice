#include "reg51.h"			 //此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

//--定义使用的IO口--//
#define GPIO_DIG   P0	//段选
#define GPIO_PLACE P2	//位选	


//--定义全局变量--//
#define NSYELLOWBlingBlingOn   1	
#define EWYELLOWBlingBlingOn   2	
#define YELLOWBlingBlingOff    3

u8 code DIG_PLACE[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//位选控制
u8 code DIG_CODE[17] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码
u8 DisplayData[8];//用来存放要显示的8位数的值 
u8 Second;
u8 YellowBlingBlingStatus=YELLOWBlingBlingOff;

		   
//--定义按键的IO口--//
sbit KEY = P3^2;

//--定义红绿灯对应的IO口--//
sbit RED_LNS   = P3^0;	 //南北人行道红灯
sbit GREEN_LNS = P3^1;	 //南北人行道绿灯
sbit RED_NS   = P1^2;    //南北红灯
sbit YELLOW_NS= P1^3;    //南北黄灯
sbit GREEN_NS = P1^4;    //南北绿灯

sbit RED_LEW   = P1^0;   //东西人行道红灯
sbit GREEN_LEW = P1^1;   //东西人行道绿灯
sbit RED_EW   = P1^5;   //东西红灯
sbit YELLOW_EW= P1^6;   //东西黄灯
sbit GREEN_EW = P1^7;   //东西绿灯

/*******************************************************************************
* 函 数 名         : delay1ms
* 函数功能		   : 延时1ms
*******************************************************************************/
void delay1ms(u16 c)   //误差 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}

/*******************************************************************************
* 函 数 名         : DigDisplay
* 函数功能		   : 数码管动态扫描函数，循环扫描8个数码管显示
*******************************************************************************/
void DigDisplay()
{
	u8 i;
	u16 j;
	for(i=0; i<8; i++)
	{
		GPIO_PLACE = DIG_PLACE[i];	 //发送位选
		GPIO_DIG = DisplayData[i];   //发送段码
		j = 10;						 //扫描间隔时间设定
		while(j--);	
		GPIO_DIG = 0x00;//消隐
	}
}

/*******************************************************************************
* 函 数 名         : Timer0Init
* 函数功能		   : 定时器0初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer0Init()
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1，仅用TR0打开启动。
	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;	
	PT0=1;
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器			
}

/*******************************************************************************
* 函 数 名         : Light_Off
* 函数功能		   : 关掉所有的灯
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Light_Off()
{
	YellowBlingBlingStatus=YELLOWBlingBlingOff;//关闭黄灯闪烁
	RED_NS = 1;         //南北红灯灭
	RED_LNS = 1;        //南北人行道红灯灭
	GREEN_NS = 1;       //南北绿灯灭			
	GREEN_LNS = 1;      //南北人行道绿灯灭
	YELLOW_NS = 1;      //南北黄灯灭

	RED_EW = 1;         //东西红灯灭
	RED_LEW = 1;        //东西人行道红灯灭
	GREEN_EW = 1;       //东西绿灯灭	
	GREEN_LEW = 1;      //东西人行道绿灯灭
	YELLOW_EW = 1;      //东西黄灯灭
}

/*******************************************************************************
* 函 数 名         : RedLight_On
* 函数功能		   : 所有红灯亮
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Red_Light_On()
{
	RED_NS = 0;         //南北红灯亮
	RED_LNS = 0;        //南北人行道红灯亮
	GREEN_NS = 1;       //南北绿灯灭			
	GREEN_LNS = 1;      //南北人行道绿灯灭
	YELLOW_NS = 1;      //南北黄灯灭

	RED_EW = 0;         //东西红灯亮
	RED_LEW = 0;        //东西人行道红灯亮
	GREEN_EW = 1;       //东西绿灯灭	
	GREEN_LEW = 1;      //东西人行道绿灯灭
	YELLOW_EW = 1;      //东西黄灯灭
}

/*******************************************************************************
* 函 数 名         : NS_Pass
* 函数功能		   : 南北通行
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void NS_Pass()
{
	RED_NS = 1;         //南北红灯灭
	RED_LNS = 1;        //南北人行道红灯灭
	GREEN_NS = 0;       //南北绿灯亮			
	GREEN_LNS = 0;      //南北人行道绿灯亮
	YELLOW_NS = 1;      //南北黄灯灭

	RED_EW = 0;         //东西红灯亮
	RED_LEW = 0;        //东西人行道红灯亮
	GREEN_EW = 1;       //东西绿灯灭	
	GREEN_LEW = 1;      //东西人行道绿灯灭
	YELLOW_EW = 1;      //东西黄灯灭
}

/*******************************************************************************
* 函 数 名         : NS_To_EW
* 函数功能		   : 南北通行转东西通行
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void NS_To_EW()
{
	RED_NS = 1;         //南北红灯灭
	RED_LNS = 1;        //南北人行道红灯灭
	GREEN_NS = 1;       //南北绿灯灭			
	GREEN_LNS = 1;      //南北人行道绿灯灭

	RED_EW = 0;         //东西红灯亮
	RED_LEW = 0;        //东西人行道红灯亮
	GREEN_EW = 1;       //东西绿灯灭	
	GREEN_LEW = 1;      //东西人行道绿灯灭
	YellowBlingBlingStatus=NSYELLOWBlingBlingOn;//南北黄灯闪烁 
}

/*******************************************************************************
* 函 数 名         : EW_Pass
* 函数功能		   : 东西通行
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EW_Pass()
{
	RED_NS = 0;         //南北红灯亮
	RED_LNS = 0;        //南北人行道红灯亮
	GREEN_NS = 1;       //南北绿灯灭			
	GREEN_LNS = 1;      //南北人行道绿灯灭
	YELLOW_NS = 1;      //南北黄灯灭

	RED_EW = 1;         //东西红灯灭
	RED_LEW = 1;        //东西人行道红灯灭
	GREEN_EW = 0;       //东西绿灯亮	
	GREEN_LEW = 0;      //东西人行道绿灯亮
	YELLOW_EW = 1;      //东西黄灯灭
}
/*******************************************************************************
* 函 数 名         : EW_To_NS
* 函数功能		   : 东西通行转南北通行
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EW_To_NS()
{
	RED_NS = 0;         //南北红灯亮
	RED_LNS = 0;        //南北人行道红灯亮
	GREEN_NS = 1;       //南北绿灯灭			
	GREEN_LNS = 1;      //南北人行道绿灯灭

	RED_EW = 1;         //东西红灯灭
	RED_LEW = 1;        //东西人行道红灯灭
	GREEN_EW = 1;       //东西绿灯灭	
	GREEN_LEW = 1;      //东西人行道绿灯灭
	YellowBlingBlingStatus=EWYELLOWBlingBlingOn;//东西黄灯闪烁
}


/*******************************************************************************
* 函 数 名       : Init
* 函数功能		 : 初始化
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void Init()
{
	Second = 1;
	Timer0Init();
	IT0=1;       //下降沿触发
    EX0=1;
    EA=1;
	YellowBlingBlingStatus=YELLOWBlingBlingOff;
	while (Second <= 5)
	{
		DisplayData[0] = DIG_CODE[(6 - Second) % 100 / 10];
		DisplayData[1] = DIG_CODE[(6 - Second) %10];
		DisplayData[2] = DisplayData[0];
		DisplayData[3] = DisplayData[1];
		DisplayData[4] = DisplayData[0];
		DisplayData[5] = DisplayData[1];
		DisplayData[6] = DisplayData[0];
		DisplayData[7] = DisplayData[1];
		DigDisplay();
		Red_Light_On();
	}
	Light_Off();
	Second = 1; 
}

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	Init();
	while(1)
	{
		if(Second == 30)Second = 1;//30秒一个循环

		//--南北通行，10秒--//
		if(Second < 10)
		{
			DisplayData[0] = DIG_CODE[(10 - Second) % 100 / 10];
			DisplayData[1] = DIG_CODE[(10 - Second) %10];
			DisplayData[2] = DisplayData[0];
			DisplayData[3] = DisplayData[1];
			DisplayData[4] = DisplayData[0];
			DisplayData[5] = DisplayData[1];
			DisplayData[6] = DisplayData[0];
			DisplayData[7] = DisplayData[1];
			DigDisplay();
			NS_Pass();//南北通行
		}
		//--黄灯等待切换状态，5秒--//
		else if(Second < 15) 
		{
			DisplayData[0] = DIG_CODE[(15 - Second) % 100 / 10];
			DisplayData[1] = DIG_CODE[(15 - Second) %10];
			DisplayData[2] = DisplayData[0];
			DisplayData[3] = DisplayData[1];
			DisplayData[4] = DisplayData[0];
			DisplayData[5] = DisplayData[1];
			DisplayData[6] = DisplayData[0];
			DisplayData[7] = DisplayData[1];
			DigDisplay();

			//--黄灯阶段--//
			NS_To_EW();
		}

		//--东西通行--//
		else if(Second < 25) 
		{
			DisplayData[0] = DIG_CODE[(25 - Second) % 100 / 10];
			DisplayData[1] = DIG_CODE[(25 - Second) %10];
			DisplayData[2] = DisplayData[0];
			DisplayData[3] = DisplayData[1];
			DisplayData[4] = DisplayData[0];
			DisplayData[5] = DisplayData[1];
			DisplayData[6] = DisplayData[0];
			DisplayData[7] = DisplayData[1];
			DigDisplay();
			EW_Pass();//南北通行

		}

		//--黄灯等待切换状态，5秒--//
		else 
		{
			DisplayData[0] = DIG_CODE[(30 - Second) % 100 / 10];
			DisplayData[1] = DIG_CODE[(30 - Second) %10];
			DisplayData[2] = DisplayData[0];
			DisplayData[3] = DisplayData[1];
			DisplayData[4] = DisplayData[0];
			DisplayData[5] = DisplayData[1];
			DisplayData[6] = DisplayData[0];
			DisplayData[7] = DisplayData[1];
			DigDisplay();
			//--黄灯阶段--//
			EW_To_NS();
		}
	}					
}


/*******************************************************************************
* 函 数 名         : void Timer0() interrupt 1
* 函数功能		   : 定时器0中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer0() interrupt 1
{
	static u16 i;
	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;
	i++;
	switch(YellowBlingBlingStatus)
	{
		case NSYELLOWBlingBlingOn:if(Second%2==0){YELLOW_NS = 0;break;}else {YELLOW_NS = 1;break;}
		case EWYELLOWBlingBlingOn:if(Second%2==0){YELLOW_EW = 0;break;}else {YELLOW_EW = 1;break;} 
		case YELLOWBlingBlingOff:YELLOW_NS = 1;	YELLOW_EW = 1;break;
		default:YELLOW_NS = 1;YELLOW_EW = 1;
	}
	if(i==1000)
	{
		i=0;
		Second ++;
		YELLOW_NS = 1;      //南北黄灯灭
		YELLOW_EW = 1;      //东西黄灯灭
		YellowBlingBlingStatus=YELLOWBlingBlingOff;	
	}	
}
/********************************************************
* INT0中断函数                                          *
********************************************************/
void  EM() interrupt 0 
{	
	u8 Second_save;
	if (KEY == 0)		//检测按键是否按下
	{
		delay1ms(10);	//消除抖动
		if (KEY == 0)	//再次检测按键是否按下
		{
			Second_save = Second;
			Second=1;
			while(1)
			if(Second < 10)
			{
				DisplayData[0] = DIG_CODE[(9 - Second) % 100 / 10];
				DisplayData[1] = DIG_CODE[(9 - Second) %10];
				DisplayData[2] = DisplayData[0];
				DisplayData[3] = DisplayData[1];
				DisplayData[4] = DisplayData[0];
				DisplayData[5] = DisplayData[1];
				DisplayData[6] = DisplayData[0];
				DisplayData[7] = DisplayData[1];
				DigDisplay();Red_Light_On();

			}else break;
			Second = Second_save;
		}
	}
}
