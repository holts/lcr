#ifndef   CONFIG_H 
#define   CONFIG_H 


sfr WDT_CONTR = 0xC1;

//==================================EEPROW偏程==============================
sfr IAP_data  = 0xC2;
sfr IAP_addrH = 0xC3;
sfr IAP_addrL = 0xC4;
sfr IAP_cmd   = 0xC5;
sfr IAP_trig  = 0xC6;
sfr IAP_contr = 0xC7;

//==============低频信号DDS相关参数====================


/* 端口设置　*/
sfr P1M1=0x91; //P1端口设置寄存器
sfr P1M0=0x92; //P1端口设置寄存器
sfr P0M1=0x93; //P0端口设置寄存器
sfr P0M0=0x94; //P0端口设置寄存器
sfr P2M1=0x95; //P2端口设置寄存器
sfr P2M0=0x96; //P2端口设置寄存器
sfr P3M1=0xB1; //P3端口设置寄存器
sfr P3M0=0xB2; //P3端口设置寄存器


sbit spk = P2^3; //蜂鸣器
sbit Kb = P2^1;  //量程开关B
sbit Ka = P2^2;  //量程开关A
sbit K3 = P1^7;
sbit K4 = P1^6;
sbit K5 = P1^5;  //7.8kHz滤波开关
sbit K6 = P1^4;
sbit K8 = P2^0;  //100Hz滤波开关



//功能取值索引
#define FUN_R		0	//电阻功能
#define FUN_C		1	//电容功能
#define FUN_L		2	//电感功能
#define FUN_S		3	//长按功能键

//量程取值索引
#define R40		0       //
#define R1K		1       //
#define R10K		2       //
#define R100K		3       //
#define AUTO		4	//长按功能键

//频率取值索引
#define F100		0       //100HZ
#define F1K		1       //1K
#define F7K8		2       //7.8K
                
//模式取值索引
#define MODE_S          0       //串联模式
#define MODE_P          1       //并联模式
#define MODE_M          2       //长按记忆

//增益取值索引
#define GAIN1           0       //1倍
#define GAIN3           1       //3倍
#define GAIN9           2       //9倍
#define GAIN27          3       //27倍

//测量植索引
#define VX_A            0       //上臂X分量
#define VY_B            1       //上臂Y分量
#define VX_C            2       //下臂X分量
#define VY_D            3       //下臂Y分量
#define GAIN_U          4       //上臂增益
#define GAIN_D          5       //下臂增益

#endif
