#ifndef   CONFIG_H 
#define   CONFIG_H 

//#include <reg52.h>
#include "stc12c5a.h"

/***********************************************************************
Note     : 系统时钟　(32MHz)
***********************************************************************/
#define CPU_FOSC        32000000  

/***********************************************************************
Note     : 看门狗设置
***********************************************************************/
sfr WDT_CONTR = 0xC1;

/***********************************************************************
Note     : EEPROW设置
***********************************************************************/
sfr ISP_DATA  = 0xC2;
sfr ISP_ADDRH = 0xC3;
sfr ISP_ADDRL = 0xC4;
sfr ISP_CMD   = 0xC5;
sfr ISP_TRIG  = 0xC6;
sfr ISP_CONTR = 0xC7;


/***********************************************************************
Note     : 端口设置
***********************************************************************/
sfr P1M1=0x91; //P1端口设置寄存器
sfr P1M0=0x92; //P1端口设置寄存器
sfr P0M1=0x93; //P0端口设置寄存器
sfr P0M0=0x94; //P0端口设置寄存器
sfr P2M1=0x95; //P2端口设置寄存器
sfr P2M0=0x96; //P2端口设置寄存器
sfr P3M1=0xB1; //P3端口设置寄存器
sfr P3M0=0xB2; //P3端口设置寄存器


/***********************************************************************
Note     : AD转换
***********************************************************************/
sfr P1ASF = 0x9D;     //将P1置为模拟口寄存器(使能),各位中为1的有效
sfr ADC_CONTR = 0xBC; //A/D转换控制寄存器
sfr ADC_RES   = 0xBD; //A/D转换结果寄存器
sfr ADC_RESL  = 0xBE; //A/D转换结果寄存器

/***********************************************************************
Note     : PCA相关寄存器
***********************************************************************/
sfr CMOD = 0xD9;   //钟源选择控制等
sfr CH = 0xF9;     //PCA的计数器
sfr CL = 0xE9;     //PCA的计数器
sfr CCON = 0xD8;   //PCA控制寄存器
sfr CCPAM0 = 0xDA; //PCA模块0工作模式寄存器
sfr CCPAM1 = 0xDB; //PCA模块1工作模式寄存器
sfr CCAP0L = 0xEA; //模块0捕获寄存器低位
sfr CCAP0H = 0xFA; //模块0捕获寄存器高位
sfr IPH = 0xB7;

sbit PPCA  = IP^7;   //PCA的中断优先级设置
sbit CCF0  = CCON^0; //PCA的模块0中断标志
sbit CCF1  = CCON^1; //PCA的模块1中断标志
sbit CR = CCON^6;    //PCA计数器使能

sbit KFB = P1^2;//移相方波输出口
sbit K32 = P1^1; //32kHz发生器

/***********************************************************************
Note     : 开关设置
***********************************************************************/
sbit spk = P2^3; //蜂鸣器
sbit Kb = P2^1;  //量程开关B
sbit Ka = P2^2;  //量程开关A
sbit K3 = P1^7;
sbit K4 = P1^6;
sbit K5 = P1^5;  //7.8kHz滤波开关
sbit K6 = P1^4;
sbit K8 = P2^0;  //100Hz滤波开关

/***********************************************************************
Note     : 键盘设置
***********************************************************************/
#define KEYPort      ~P3^0x1F
#define KEY_M        0x01
#define KEY_X        0x02
#define KEY_R        0x03
#define KEY_F        0x04
#define KEY_C        0x05

/***********************************************************************
Note     : 常量设置
***********************************************************************/

//功能取值索引
#define FUN_R		0  	  //电阻功能
#define FUN_C		1	  //电容功能
#define FUN_L		2	  //电感功能
#define FUN_S		3	  //长按功能键

//量程取值索引
#define R40		0         //
#define R1K		1         //
#define R10K		2         //
#define R100K		3         //
#define AUTO		4	  //长按功能键

//频率取值索引
#define F100		0         //100HZ
#define F1K		1         //1K
#define F7K8		2         //7.8K
                
//模式取值索引
#define MODE_S          0         //串联模式
#define MODE_P          1         //并联模式
#define MODE_M          2         //长按记忆

//增益取值索引
#define GAIN1           0         //1倍
#define GAIN3           1         //3倍
#define GAIN9           2         //9倍
#define GAIN27          3         //27倍
#define GAIN_UP         4         //上臂增益
#define GAIN_DOWN       5         //下臂增益

//测量植索引
#define VX_A            0       //上臂X分量
#define VY_B            1       //上臂Y分量
#define VX_C            2       //下臂X分量
#define VY_D            3       //下臂Y分量

//ADC上下限
#define VFULL           9600
#define GAD             (9600/30)

//SEM InitSem(meter_fine)
#define  DISPLAY   0
#define  METER     1
#define  STOP      2
#define  SETUP     3


#endif
