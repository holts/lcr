
#include "config.h"
#include "key.h"
#include "adc.h"
#include "pwm.h"
#include "eeprom.h"
#include "mylcr.h"



extern Trg;
extern Cont;
extern cnt_plus;

unsigned char  Status=STOP;          //系统状态

xdata unsigned char Fun_idx=FUN_R;   //当前功能索引
xdata unsigned char Rang_idx=R40;    //当前量程索引
xdata unsigned char Frq_idx=F1K;     //当前频率索引
xdata unsigned char Mode_idx=MODE_S; //当前模式索引

xdata unsigned char Gain_idx=GAIN1;  //当前增益索引
xdata unsigned char Vxy_idx=VX_A;    //当前测量索引


//测量结果
xdata int Vxy[6]={0,0,0,0,1,1}; //Vxy[Vx_a,Vy_b,Vx_c,Vy_d,g_up,g_down]
//修正参数
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
void WDTC() { WDT_CONTR =0x3C; }

/***********************************************************************
Function : INTT0 interrupt
Note     : 定时器中断处理函数
***********************************************************************/
void INTT0(void) interrupt 1 using 1
{
	TH0 = 0xF5, TL0 = 0x95;    //1ms定时重装
        //UpdateTimers();
}

/***********************************************************************
Function : timerInter1
Note     : T1中断，LCR数据采集
***********************************************************************/
void timerInter1(void) interrupt 3 
{
	int c=0;
	TH1 = 0x96, TL1 = 0xAB;      //20ms定时重装
 
	if (Status == METER)
	{
	  Vxy[Vxy_idx] = ADC_Read();   //读取电压值

	  if ((Vxy_idx==VX_A)||(Vxy_idx==VX_C))
	  {//电压模值才能反应运放的输出幅度，所以增益切换判断得用模值
	    if (Vxy_idx==VX_A) c = absMax(Vxy[VX_C],Vxy[VY_D]); //切换到下臂
	    if (Vxy_idx==VX_C) c = absMax(Vxy[VX_A],Vxy[VY_B]); //切换到上臂

  	    if (c>VFULL) { setGain(Gain_idx--);}  //减小增益
	    else if (c<GAD*1) {setGain(GAIN27);}  //增加27倍
	    else if (c<GAD*3) {setGain(GAIN9);}   //增加9倍
	    else if (c<GAD*9) {setGain(GAIN3);}   //增加3倍

	    if (Vxy_idx==VX_A) Vxy[GAIN_DOWN] = Gain_idx;  //记录下臂增益
            if (Vxy_idx==VX_C) Vxy[GAIN_UP] = Gain_idx;    //记录上臂增益
	   }
 
	  if (Vxy_idx == VY_D) Status = DISPLAY;

 	  Vxy_idx = (Vxy_idx + 1)%4;

	  if (Vxy_idx==VX_A) K3=1;  //切换到上臂
	  if (Vxy_idx==VX_C) K3=0;  //切换到下臂
	}

	KeyRead();
    
    if (Trg&KEY_F) {setFrq(Frq_idx);}  //设置频率
}

/***********************************************************************
Function : Time_Init
Note     : 定时器初始化 
***********************************************************************/
void Time_Init(void)
{     
     TCON=0, TMOD=0x11; //将T0和T1置为16重装位定时器
     TH1 = 0x96, TL1 = 0xAB;    //20ms定时
     TR1=1;  //T1开始计数
     TH0 = 0xF5, TL0 = 0x95;    //1ms定时
     TR0=0;  //T0暂停计数
	
     ET1=1;  //T1开中断
     ET0=1;  //T0开中断
}

/***********************************************************************
Function : Main
Note     : 主程序
***********************************************************************/
void main()
{
    Time_Init();    //定时器初始化

    EA=1;           //开总中断

    ADC_Init(0);    //设置AD转换通道为P1.0

    P1M0 = 0xFC;    //P1.234567置为推换口
    P1M1 = 0x03;    //P1.01置为高阻抗
    P2M0 = 0xFF;    //P2.01234567置为推勉输出
    P2 = 0x0F; 

    PWM_Init();     //DDS初始化 把PCA置为PWM
	
    set90(2);       //初始设置相位
    setRng(Rang_idx);  //初始设置量程
    setGain(Gain_idx); //初始设置增益
    setFrq(Frq_idx);   //DDS初始设置为1kHz	

    //LCD_INIT();

    //LCD_PrintString("LCR 3.0");
    //LCD_2ndRow();
    //LCD_PrintString("XJW Putian, 2012");

    while(1)
     {
  	   WDTC(); 

	   //键盘处理 
   	   if (Trg&KEY_M) {M_Done(Fun_idx);}  //功能切换
   	   if (Trg&KEY_X) {X_Done(Mode_idx);} //模式切换
   	   if (Trg&KEY_R) {setRng(0);}        //量程步进
   	   if (Trg&KEY_C) {C_Done(0);}        //清零
   	   if (Cont&KEY_M)
     	   {
       		cnt_plus++;
       		if (cnt_plus>100)  {cnt_plus=0;M_Done(FUN_S);}
     	    } 
   	   if (Cont&KEY_R)
     	   {
       		cnt_plus++;
       		if (cnt_plus>100) {cnt_plus=0;setRng(1);} 
     	    }
   	   if (Cont&KEY_C)
     	   {
       		cnt_plus++;
       		if (cnt_plus>100) {cnt_plus=0;C_Done(1);}
     	    }
   	   if (!(Cont)){cnt_plus=0;}

	   //显示处理
	   if (Status == DISPLAY ) 
	    {
	        Status = METER;

		   //calcLCR();
		   //SendSem(meter_fine);
		   //showR();
              } 
     }
}

