
#include <reg52.h>
#include "config.h"
#include "ptask.h"
#include "mylcr.h"
#include "key.h"
#include "adc.h"
#include "pwm.h"
#include "hd44780.h"

SEM InitSem(meter_fine)

xdata unsigned char Fun_idx=FUN_R;   //当前功能索引
xdata unsigned char Rang_idx=R40;    //当前量程索引
xdata unsigned char Frq_idx=F1K;     //当前频率索引
xdata unsigned char Mode_idx=MODE_S; //当前模式索引
xdata unsigned char Gain_idx=GAIN1;  //当前增益索引
xdata unsigned char Vxy_idx=VX_A;    //当前测量索引

//增益表
code float gainB[4] =  {  1,   3,   9,  27 };
//各档电阻表
code float resistorB[4] = { 40, 1e3, 1e4, 1e5 };
//测量结果
xdata int Vxy[12]={0,0,0,0,1,1}; //Vxy[Vx_a,Vy_b,Vx_c,Vy_d,g1,g2]
//测量计数

xdata struct Ida{
 char zo[3];//三个频率下的零点改正值
 char j1;   //相位补偿(3倍档)
 char j2;   //相位补偿(10倍档)
 char J[4];  //相位补偿(V/I变换器)
 char R[4]; //下臂电阻修正(40,1k,10k,100k)
 char g1;   //增益修正(3倍档)
 char g2;   //增益修正(10倍档)
 char phx; //1kHz以下相位改正
 char R4b; //100k档7.8kHz频率下的幅度补偿
 char G2b; //9倍档7.8kHz频率下的幅度补偿
 char feq; //频率修正
 char ak;  //AD斜率修正
 float QRs[3],QXs[3]; //短路清零数据
 float QRo[3],QXo[3]; //开路清零数据
} cs;

/***********************************************************************
Function : WDTC
Note     : 清除看门狗
***********************************************************************/
void WDTC()
{ WDT_CONTR =0x3C; }

/***********************************************************************
Function : INTT0 interrupt
Note     : 定时器中断处理函数
Updata   : 2013-06-18
***********************************************************************/
void INTT0(void) interrupt 1 using 1
{
	TH0 = 0xF5, TL0 = 0x95;    //1ms定时重装
        UpdateTimers();
}


/***********************************************************************
Function : setGain
Note     : 设置电路增益
***********************************************************************/
void setGain(unsigned char switch_sts){
  if (switch_sts!=Gain_idx) { Gain_idx=switch_sts; }
  else  { Gain_idx = (Gain_idx+1)%4; }
 
  K4 = Gain_idx & 2, K6 = Gain_idx & 1; //1倍--27倍
}


/***********************************************************************
Function : Init
Note     : 初始化 
***********************************************************************/
void Init(void)
{	//定时器初始化
        TCON=0, TMOD=0x11; //将T0置为16位定时器，T1置为16位定时器
        TH1 = 0, TL1 = 0;
        TR1=1;  //T1开始计数
	TH0 = 0xF5, TL0 = 0x95;    //1ms定时
        TR0=1;  //T0开始计数
 	ET1=1;  //T1开中断
	ET0=1;  //T0开中断

	ADC_Init(0);    //设置AD转换通道为P1.0

 	P1M0 = 0xFC;    //P1.234567置为推换口
 	P1M1 = 0x03;    //P1.01置为高阻抗
 	P2M0 = 0xFF;    //P2.01234567置为推勉输出
 	P2 = 0x0F; 

	PWM_Init();   //DDS初始化 把PCA置为PWM
	
	EA=1;   //开总中断

 	set90(2);   //初始设置相位
 	setRng(Rang_idx);  //初始设置量程
 	setGain(Gain_idx); //初始设置增益
 	setFrq(Frq_idx);    //DDS初始设置为1kHz	

	LCD_INIT();

	LCD_PrintString("LCR 3.0");
        LCD_2ndRow();
	LCD_PrintString("XJW Putian, 2012");
}

/***********************************************************************
Function : calcLCR
Note     : 计算LCR
***********************************************************************/
void calcLCR(void){
	xdata float a=0,b=0,c=0;

	a =  +(Vxy[VX_C] * Vxy[VX_C] + Vxy[VY_D] * Vxy[VY_D] );
	b =  -( Vxy[VX_A] * Vxy[VX_C] + Vxy[VY_B] * Vxy[VY_D]);
	c =  -(Vxy[VY_B] * Vxy[VX_C] - Vxy[VX_A] * Vxy[VY_D]);
	
	a /= resistorB[Rang_idx];  //除以下臂电阻阻值
 
}

/***********************************************************************
Function : timerInter1
Note     : T1中断，LCR数据采集
***********************************************************************/
void timerInter1(void) interrupt 3 {
	Vxy[Vxy_idx] = ADC_Read();

//	WaitSem(meter_fine);
 	Vxy_idx = (Vxy_idx + 1)%4;
	set90(Vxy_idx);
	if (Vxy_idx==VX_A) K3=1;
	if (Vxy_idx==VX_C) K3=0;

}

/***********************************************************************
Function : showR
Note     : 显示LCR
***********************************************************************/
void showR(void){ 
	//显示频率
	LCD_Locate(2,0);
	if (Frq_idx==F100) LCD_PrintChar('A');
	if (Frq_idx==F1K) LCD_PrintChar('B');
	if (Frq_idx==F7K8) LCD_PrintChar('C');
	//显示量程
	if (Mode_idx==MODE_S)//串联
	{
		LCD_Locate(1,0);
		LCD_PrintString("Zp");
	}
	if (Mode_idx==MODE_P)//并联
	{
		LCD_Locate(1,0);
		LCD_PrintString("Zs");
	}
	  
}

/***********************************************************************
Function : Main
Note     : 主程序
***********************************************************************/
void main()
{
    Init();
    	
	while(1)
	{
  	   WDTC(); 
	   RunTaskA(KeyProc,Task1_ID);
	   if (meter_fine) 
	   {
		   calcLCR();
		   SendSem(meter_fine);
	   }
	   showR();
	}
}

