
#include <stdio.h>
#include "config.h"

extern Fun_idx=FUN_R;   //当前功能索引
extern Rang_idx=R40;    //当前量程索引
extern Frq_idx=F1K;     //当前频率索引
extern Mode_idx=MODE_S; //当前模式索引
extern Gain_idx=GAIN1;  //当前增益索引
extern Vxy_idx=VX_A;    //当前测量索引

extern Actual_Frq;
extern Vxy[];

//增益表
code unsigned char gainB[4] =  {  1,   3,   9,  27 };
//各档电阻表
code float resistorB[4] = { 40, 1e3, 1e4, 1e5 };


/***********************************************************************
Function : Fmt
Note     : 显示LCR
***********************************************************************/
void Fmt(float a)
{
	unsigned char showString[20];
	sprintf(showString, "%.2f", a);
}

/***********************************************************************
Function : showR
Note     : 显示LCR
***********************************************************************/
void showR(void){ 
	//显示频率
	//LCD_Locate(2,0); 
	//if (Frq_idx==F100) LCD_PrintChar('A');
	//if (Frq_idx==F1K) LCD_PrintChar('B');
	//if (Frq_idx==F7K8) LCD_PrintChar('C');
	//显示量程
	//LCD_PrintChar(Rang_idx+49);

	//LCD_Locate(1,0);
	if (Mode_idx==MODE_S)//串联
	{
		//LCD_PrintString("Zs");

		//LCD_PrintString('R'); 

		//LCD_Locate(2,4);
	}
	if (Mode_idx==MODE_P)//并联
	{
		//LCD_PrintString("Zp");
	}
	  
}



/***********************************************************************
Function : calcLCR
Note     : 计算LCR
***********************************************************************/
void calcLCR(void){
	xdata float a=0,b=0,c=0;
	xdata float w=0;

	a =  +(Vxy[VX_C] * Vxy[VX_C] + Vxy[VY_D] * Vxy[VY_D] );
	b =  -( Vxy[VX_A] * Vxy[VX_C] + Vxy[VY_B] * Vxy[VY_D]);
	c =  -(Vxy[VY_B] * Vxy[VX_C] - Vxy[VX_A] * Vxy[VY_D]);
	
	a /= resistorB[Rang_idx];  //除以下臂电阻阻值
 
	w = 2*3.1415926* Actual_Frq;
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
Function : absMax
Note     : 取两个数绝对值最大者
***********************************************************************/
int absMax(int a,int b)
{
  if(a<0) a = -a;
  if(b<0) b = -b;
  if(b>a) a = b;
  return a;
}
