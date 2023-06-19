#include "reg51.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

//--����ʹ�õ�IO��--//
#define GPIO_DIG   P0	//��ѡ
#define GPIO_PLACE P2	//λѡ	


//--����ȫ�ֱ���--//
#define NSYELLOWBlingBlingOn   1	
#define EWYELLOWBlingBlingOn   2	
#define YELLOWBlingBlingOff    3

u8 code DIG_PLACE[8] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//λѡ����
u8 code DIG_CODE[17] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0��1��2��3��4��5��6��7��8��9��A��b��C��d��E��F����ʾ��
u8 DisplayData[8];//�������Ҫ��ʾ��8λ����ֵ 
u8 Second;
u8 YellowBlingBlingStatus=YELLOWBlingBlingOff;

		   
//--���尴����IO��--//
sbit KEY = P3^2;

//--������̵ƶ�Ӧ��IO��--//
sbit RED_LNS   = P3^0;	 //�ϱ����е����
sbit GREEN_LNS = P3^1;	 //�ϱ����е��̵�
sbit RED_NS   = P1^2;    //�ϱ����
sbit YELLOW_NS= P1^3;    //�ϱ��Ƶ�
sbit GREEN_NS = P1^4;    //�ϱ��̵�

sbit RED_LEW   = P1^0;   //�������е����
sbit GREEN_LEW = P1^1;   //�������е��̵�
sbit RED_EW   = P1^5;   //�������
sbit YELLOW_EW= P1^6;   //�����Ƶ�
sbit GREEN_EW = P1^7;   //�����̵�

/*******************************************************************************
* �� �� ��         : delay1ms
* ��������		   : ��ʱ1ms
*******************************************************************************/
void delay1ms(u16 c)   //��� 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}

/*******************************************************************************
* �� �� ��         : DigDisplay
* ��������		   : ����ܶ�̬ɨ�躯����ѭ��ɨ��8���������ʾ
*******************************************************************************/
void DigDisplay()
{
	u8 i;
	u16 j;
	for(i=0; i<8; i++)
	{
		GPIO_PLACE = DIG_PLACE[i];	 //����λѡ
		GPIO_DIG = DisplayData[i];   //���Ͷ���
		j = 10;						 //ɨ����ʱ���趨
		while(j--);	
		GPIO_DIG = 0x00;//����
	}
}

/*******************************************************************************
* �� �� ��         : Timer0Init
* ��������		   : ��ʱ��0��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Timer0Init()
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������
	TH0=0XFC;	//����ʱ������ֵ����ʱ1ms
	TL0=0X18;	
	PT0=1;
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	TR0=1;//�򿪶�ʱ��			
}

/*******************************************************************************
* �� �� ��         : Light_Off
* ��������		   : �ص����еĵ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Light_Off()
{
	YellowBlingBlingStatus=YELLOWBlingBlingOff;//�رջƵ���˸
	RED_NS = 1;         //�ϱ������
	RED_LNS = 1;        //�ϱ����е������
	GREEN_NS = 1;       //�ϱ��̵���			
	GREEN_LNS = 1;      //�ϱ����е��̵���
	YELLOW_NS = 1;      //�ϱ��Ƶ���

	RED_EW = 1;         //���������
	RED_LEW = 1;        //�������е������
	GREEN_EW = 1;       //�����̵���	
	GREEN_LEW = 1;      //�������е��̵���
	YELLOW_EW = 1;      //�����Ƶ���
}

/*******************************************************************************
* �� �� ��         : RedLight_On
* ��������		   : ���к����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Red_Light_On()
{
	RED_NS = 0;         //�ϱ������
	RED_LNS = 0;        //�ϱ����е������
	GREEN_NS = 1;       //�ϱ��̵���			
	GREEN_LNS = 1;      //�ϱ����е��̵���
	YELLOW_NS = 1;      //�ϱ��Ƶ���

	RED_EW = 0;         //���������
	RED_LEW = 0;        //�������е������
	GREEN_EW = 1;       //�����̵���	
	GREEN_LEW = 1;      //�������е��̵���
	YELLOW_EW = 1;      //�����Ƶ���
}

/*******************************************************************************
* �� �� ��         : NS_Pass
* ��������		   : �ϱ�ͨ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void NS_Pass()
{
	RED_NS = 1;         //�ϱ������
	RED_LNS = 1;        //�ϱ����е������
	GREEN_NS = 0;       //�ϱ��̵���			
	GREEN_LNS = 0;      //�ϱ����е��̵���
	YELLOW_NS = 1;      //�ϱ��Ƶ���

	RED_EW = 0;         //���������
	RED_LEW = 0;        //�������е������
	GREEN_EW = 1;       //�����̵���	
	GREEN_LEW = 1;      //�������е��̵���
	YELLOW_EW = 1;      //�����Ƶ���
}

/*******************************************************************************
* �� �� ��         : NS_To_EW
* ��������		   : �ϱ�ͨ��ת����ͨ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void NS_To_EW()
{
	RED_NS = 1;         //�ϱ������
	RED_LNS = 1;        //�ϱ����е������
	GREEN_NS = 1;       //�ϱ��̵���			
	GREEN_LNS = 1;      //�ϱ����е��̵���

	RED_EW = 0;         //���������
	RED_LEW = 0;        //�������е������
	GREEN_EW = 1;       //�����̵���	
	GREEN_LEW = 1;      //�������е��̵���
	YellowBlingBlingStatus=NSYELLOWBlingBlingOn;//�ϱ��Ƶ���˸ 
}

/*******************************************************************************
* �� �� ��         : EW_Pass
* ��������		   : ����ͨ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EW_Pass()
{
	RED_NS = 0;         //�ϱ������
	RED_LNS = 0;        //�ϱ����е������
	GREEN_NS = 1;       //�ϱ��̵���			
	GREEN_LNS = 1;      //�ϱ����е��̵���
	YELLOW_NS = 1;      //�ϱ��Ƶ���

	RED_EW = 1;         //���������
	RED_LEW = 1;        //�������е������
	GREEN_EW = 0;       //�����̵���	
	GREEN_LEW = 0;      //�������е��̵���
	YELLOW_EW = 1;      //�����Ƶ���
}
/*******************************************************************************
* �� �� ��         : EW_To_NS
* ��������		   : ����ͨ��ת�ϱ�ͨ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EW_To_NS()
{
	RED_NS = 0;         //�ϱ������
	RED_LNS = 0;        //�ϱ����е������
	GREEN_NS = 1;       //�ϱ��̵���			
	GREEN_LNS = 1;      //�ϱ����е��̵���

	RED_EW = 1;         //���������
	RED_LEW = 1;        //�������е������
	GREEN_EW = 1;       //�����̵���	
	GREEN_LEW = 1;      //�������е��̵���
	YellowBlingBlingStatus=EWYELLOWBlingBlingOn;//�����Ƶ���˸
}


/*******************************************************************************
* �� �� ��       : Init
* ��������		 : ��ʼ��
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void Init()
{
	Second = 1;
	Timer0Init();
	IT0=1;       //�½��ش���
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
* �� �� ��       : main
* ��������		 : ������
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void main()
{	
	Init();
	while(1)
	{
		if(Second == 30)Second = 1;//30��һ��ѭ��

		//--�ϱ�ͨ�У�10��--//
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
			NS_Pass();//�ϱ�ͨ��
		}
		//--�ƵƵȴ��л�״̬��5��--//
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

			//--�Ƶƽ׶�--//
			NS_To_EW();
		}

		//--����ͨ��--//
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
			EW_Pass();//�ϱ�ͨ��

		}

		//--�ƵƵȴ��л�״̬��5��--//
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
			//--�Ƶƽ׶�--//
			EW_To_NS();
		}
	}					
}


/*******************************************************************************
* �� �� ��         : void Timer0() interrupt 1
* ��������		   : ��ʱ��0�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Timer0() interrupt 1
{
	static u16 i;
	TH0=0XFC;	//����ʱ������ֵ����ʱ1ms
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
		YELLOW_NS = 1;      //�ϱ��Ƶ���
		YELLOW_EW = 1;      //�����Ƶ���
		YellowBlingBlingStatus=YELLOWBlingBlingOff;	
	}	
}
/********************************************************
* INT0�жϺ���                                          *
********************************************************/
void  EM() interrupt 0 
{	
	u8 Second_save;
	if (KEY == 0)		//��ⰴ���Ƿ���
	{
		delay1ms(10);	//��������
		if (KEY == 0)	//�ٴμ�ⰴ���Ƿ���
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
