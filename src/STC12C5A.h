#ifndef STC12C5A_H
#define STC12C5A_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  PSW        =   0xd0;       //程序状态字            Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                CY      AC      F0      RS1     RS0     OV      F1      P
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

sbit CY         =   PSW^7;      //进位标志
sbit AC         =   PSW^6;      //辅助进位标志
sbit F0         =   PSW^5;      //用户标志
sbit RS1        =   PSW^4;      //寄存器组选择位1
sbit RS0        =   PSW^3;      //寄存器组选择位0
sbit OV         =   PSW^2;      //溢出标志
sbit P          =   PSW^0;      //ACC的偶校验位

/////////////////////////////////

sfr  ACC        =   0xe0;       //累加器                Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

sbit ACC7       =   ACC^7;      //累加器第7位
sbit ACC6       =   ACC^6;      //累加器第6位
sbit ACC5       =   ACC^5;      //累加器第5位
sbit ACC4       =   ACC^4;      //累加器第4位
sbit ACC3       =   ACC^3;      //累加器第3位
sbit ACC2       =   ACC^2;      //累加器第2位
sbit ACC1       =   ACC^1;      //累加器第1位
sbit ACC0       =   ACC^0;      //累加器第0位

/////////////////////////////////

sfr  B          =   0xf0;       //B寄存器               Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  SP         =   0x81;       //堆栈指针              Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0111      0       0       0       0       0       1       1       1

/////////////////////////////////

sfr  DPL        =   0x82;       //数据指针低字节        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  DPH        =   0x83;       //数据指针高字节        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  PCON       =   0x87;       //电源控制寄存器        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                SMOD    SMOD0   LVDF    POF     GF1     GF0     PD      IDL
                                //初始值=0011,0000      0       0       1       1       0       0       0       0

#define SMOD        0x80        //串口波特率倍速位,置1可使波特率快1倍
#define SMOD0       0x40        //FE/SM0选择位,0:SCON.7为SM0 1:SCON.7为FE
#define LVDF        0x20        //低压检测中断请求位,由硬件置1,需由软件清0
#define POF         0x10        //上电复位标志位,上电时由硬件置1,需由软件清0
#define GF1         0x08        //通用标志位1
#define GF0         0x04        //通用标志位0
#define PD          0x02        //掉电控制位,写1可以使MCU进入PowerDown模式
#define IDL         0x01        //空闲控制位,写1可以使MCU进入Idle模式

/////////////////////////////////

sfr  WAKE_CLKO  =   0x8f;       //掉电唤醒/时钟输出控制 Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述          PCAWAKEUP RXDPINIE T1PINIE T0PINIE LVD_WAKEUP BRTCLKO T1VLKO  T0CLKO
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

#define PCAWAKEUP   0x80        //允许PCA的上升沿/下降沿中断唤醒掉电MCU
#define RXD_PIN_IE  0x40        //允许RXD(P3.0)下降沿置位RI中断时唤醒掉电MCU(必须打开相应中断)
#define T1_PIN_IE   0x20        //允许T1(P3.5)下降沿置位T1中断时唤醒掉电MCU(必须打开相应中断)
#define T0_PIN_IE   0x10        //允许T0(P3.4)下降沿置位T0中断时唤醒掉电MCU(必须打开相应中断)
#define LVD_WAKEUP  0x08        //允许LVD(P4.6)的低压检测中断唤醒掉电MCU(必须打开相应中断)
#define BRTCLKOEN   0x04        //打开P1.0脚的BRT时钟溢出脉冲,输出的时钟频率为1/2倍的BRT溢出率
#define T1CLKOEN    0x02        //打开P3.5脚的定时器1时钟溢出脉冲,输出的时钟频率为1/2倍的T1溢出率
#define T0CLKOEN    0x01        //打开P3.4脚的定时器0时钟溢出脉冲,输出的时钟频率为1/2倍的T0溢出率

/////////////////////////////////

sfr  CLK_DIV    =   0x97;       //时钟分频寄存器        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       -       -       -       -       CLKS2   CLKS1   CLKS0
                                //初始值=xxxx,x000      x       x       x       x       x       0       0       0

#define FOSCD1      0x00        //系统时钟为Fosc
#define FOSCD2      0x01        //系统时钟为Fosc/2
#define FOSCD4      0x02        //系统时钟为Fosc/4
#define FOSCD8      0x03        //系统时钟为Fosc/8
#define FOSCD16     0x04        //系统时钟为Fosc/16
#define FOSCD32     0x05        //系统时钟为Fosc/32
#define FOSCD64     0x06        //系统时钟为Fosc/64
#define FOSCD128    0x07        //系统时钟为Fosc/128

/////////////////////////////////

sfr  BUS_SPEED  =   0xa1;       //数据总线速度控制器    Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       -       ALES1   ALES0   -       RWS2    RWS1    RWS0
                                //初始值=xx10,x011      x       x       1       0       x       0       1       1

#define ALES1       0x20        //P0地址建立时间和保持时间到ALE信号的下降沿的时钟数
#define ALES0       0x10        //ALES1/ALES0=0/0:1个时钟 0/1:2个时钟 1/0:3个时钟 1/1:4个时钟

#define RWS2        0x04        //MOVX指令的时钟数
#define RWS1        0x02        //RWS2/RWS1/RWS0=000:1个时钟 001:2个时钟 010:3个时钟 011:4个时钟
#define RWS0        0x01        //               100:5个时钟 101:6个时钟 110:7个时钟 111:8个时钟

/////////////////////////////////

sfr  WDT_CONTR  =   0xc1;       //看门狗定时器控制器    Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述             WDT_FLAG   -       EN_WDT  CLR_WDT IDL_WDT PS2     PS1     PS0
                                //初始值=0x00,0000      0       x       0       0       0       0       0       0

#define WDT_FLAG    0x80        //看门狗复位标志,当看门狗溢出产生复位后,硬件自动置1,需要由软件清0
#define EN_WDT      0x20        //软件使能看门狗,打开后不能用软件的方式关闭
#define CLR_WDT     0x10        //清看门狗定时器
#define IDLE_WDT    0x08        //IDLE模式下,看门狗定时器是否继续计时

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  AUXR       =   0x8e;       //辅助寄存器            Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                T0x12   T1x12   UM0x6   BRTR    S2SMOD  BRTx12  EXTRAM  S1BRS
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

#define T0x12       0x80        //定时器0时钟控制,0:Fosc/12(传统的12分频) 1:Fosc/1(1T模式,不分频,即12倍于传统速度)
#define T1x12       0x40        //定时器1时钟控制,0:Fosc/12(传统的12分频) 1:Fosc/1(1T模式,不分频,即12倍于传统速度)
#define UR0x6       0x20        //串口模式0的时钟控制,0:Fosc/12(传统的12分频) 1:Fosc/2(2分频,,即6倍于传统速度)
#define BRTR        0x10        //独立波特率发生器启动控制位,1:启动独立波特率发生器 0:停止独立波特率发生器
#define S2SMOD      0x08        //串口2波特率倍速位,置1可使串口2的波特率快1倍
#define BRTx12      0x04        //独立波特率发生器时钟控制,0:Fosc/12(传统的12分频) 1:Fosc/1(1T模式,不分频,即12倍于传统速度)
#define EXTRAM      0x02        //内部扩展RAM禁能位,0:内部扩展RAM有效 1:禁用内部扩展RAM
#define S1BRS       0x01        //串口1的波特率发生器选择位,0:定时器1 1:独立波特率发生器(注意串口2只能够使用独立波特率发生器)

/////////////////////////////////

sfr  AUXR1      =   0xa2;       //辅助寄存器1           Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       PCA_P4  SPI_P4  S2_P4   GF2     ADRJ    -       DPS
                                //初始值=x000,00x0      x       0       0       0       0       0       x       0

#define PCA_P4      0x40        //将PCA口映射到P4口,0:ECI(P1.2)CEX0(P1.3)CEX1(P1.4) 1:ECI(P4.1)CEX0(P4.2)CEX1(P4.3)
#define SPI_P4      0x20        //将SPI口映射到P4口,0:SCLK(P1.7)MISO(P1.6)MOSI(P1.5)SS(P1.4) 1:SCLK(P4.3)MISO(P4.2)MOSI(P4.1)SS(P4.0)
#define S2_P4       0x10        //将串口2映射到P4口,0:RXD2(P1.2)TXD2(P1.3) 1:RXD2(P4.2)TXD2(P4.3)
#define GF2         0x08        //通用标志位2
#define ADRJ        0x04        //ADC结果调整,0:{ADC_RES[9876,5432],ADC_RESL[----,--10]} 1:{ADC_RES[----,--98],ADC_RESL[7654,3210]}
#define DPS         0x01        //DPTR0/DPTR1选择位,0:DPTR0 1:DPTR1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  P0         =   0x80;       //I/O端口0              Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                P0.7    P0.6    P0.5    P0.4    P0.3    P0.2    P0.1    P0.0
                                //初始值=1111,1111      1       1       1       1       1       1       1       1

sbit P07        =   P0^7;       //I/O口P0.7
sbit P06        =   P0^6;       //I/O口P0.6
sbit P05        =   P0^5;       //I/O口P0.5
sbit P04        =   P0^4;       //I/O口P0.4
sbit P03        =   P0^3;       //I/O口P0.3
sbit P02        =   P0^2;       //I/O口P0.2
sbit P01        =   P0^1;       //I/O口P0.1
sbit P00        =   P0^0;       //I/O口P0.0

/////////////////////////////////

sfr  P1         =   0x90;       //I/O端口0              Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                P1.7    P1.6    P1.5    P1.4    P1.3    P1.2    P1.1    P1.0
                                //初始值=1111,1111      1       1       1       1       1       1       1       1

sbit P17        =   P1^7;       //I/O口P1.7
sbit P16        =   P1^6;       //I/O口P1.6
sbit P15        =   P1^5;       //I/O口P1.5
sbit P14        =   P1^4;       //I/O口P1.4
sbit P13        =   P1^3;       //I/O口P1.3
sbit P12        =   P1^2;       //I/O口P1.2
sbit P11        =   P1^1;       //I/O口P1.1
sbit P10        =   P1^0;       //I/O口P1.0

sbit ADCIN7     =   P1^7;       //ADC通道7的模拟信号输入脚
sbit ADCIN6     =   P1^6;       //ADC通道6的模拟信号输入脚
sbit ADCIN5     =   P1^5;       //ADC通道5的模拟信号输入脚
sbit ADCIN4     =   P1^4;       //ADC通道4的模拟信号输入脚
sbit ADCIN3     =   P1^3;       //ADC通道3的模拟信号输入脚
sbit ADCIN2     =   P1^2;       //ADC通道2的模拟信号输入脚
sbit ADCIN1     =   P1^1;       //ADC通道1的模拟信号输入脚
sbit ADCIN0     =   P1^0;       //ADC通道0的模拟信号输入脚

sbit SPISCLK    =   P1^7;       //SPI总线的时钟脚
sbit SPIMISO    =   P1^6;       //SPI总线的主出从入脚
sbit SPIMOSI    =   P1^5;       //SPI总线的主入从出脚
sbit SPISS      =   P1^4;       //SPI总线的从机选择脚

sbit CEX1       =   P1^4;       //比较/捕获/PWM1外部引脚
sbit CEX0       =   P1^3;       //比较/捕获/PWM0外部引脚
sbit ECI        =   P1^2;       //比较/捕获/PWM模块的外部时钟输入脚(最大值为Fosc/2)

sbit TXD2       =   P1^3;       //串口2数据输出脚
sbit RXD2       =   P1^2;       //串口2数据输入脚

sbit BRTCLKO    =   P1^0;       //BRT时钟溢出脉冲输出脚

/////////////////////////////////

sfr  P2         =   0xa0;       //I/O端口2              Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                P2.7    P2.6    P2.5    P2.4    P2.3    P2.2    P2.1    P2.0
                                //初始值=1111,1111      1       1       1       1       1       1       1       1

sbit P27        =   P2^7;       //I/O口P2.7
sbit P26        =   P2^6;       //I/O口P2.6
sbit P25        =   P2^5;       //I/O口P2.5
sbit P24        =   P2^4;       //I/O口P2.4
sbit P23        =   P2^3;       //I/O口P2.3
sbit P22        =   P2^2;       //I/O口P2.2
sbit P21        =   P2^1;       //I/O口P2.1
sbit P20        =   P2^0;       //I/O口P2.0

/////////////////////////////////

sfr  P3         =   0xb0;       //I/O端口3              Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                P3.7    P3.6    P3.5    P3.4    P3.3    P3.2    P3.1    P3.0
                                //初始值=1111,1111      1       1       1       1       1       1       1       1

sbit P37        =   P3^7;       //I/O口P3.7
sbit P36        =   P3^6;       //I/O口P3.6
sbit P35        =   P3^5;       //I/O口P3.5
sbit P34        =   P3^4;       //I/O口P3.4
sbit P33        =   P3^3;       //I/O口P3.3
sbit P32        =   P3^2;       //I/O口P3.2
sbit P31        =   P3^1;       //I/O口P3.1
sbit P30        =   P3^0;       //I/O口P3.0

sbit RXD        =   P3^0;       //串口1的数据接收口
sbit TXD        =   P3^1;       //串口1的数据发送口
sbit INT0       =   P3^2;       //外部中断0的信号输入口
sbit INT1       =   P3^3;       //外部中断1的信号输出口
sbit T0         =   P3^4;       //定时器0的外部信号输入口
sbit T1         =   P3^5;       //定时器1的外部信号输入口
sbit WR         =   P3^6;       //外部数据存储器的写信号
sbit RD         =   P3^7;       //外部数据存储器的读信号

sbit T0CLKO     =   P3^4;       //定时器0的时钟溢出脉冲输出脚
sbit T1CLKO     =   P3^5;       //定时器1的时钟溢出脉冲输出脚

/////////////////////////////////

sfr  P4         =   0xc0;       //I/O端口4              Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                P4.7    P4.6    P4.5    P4.4    P4.3    P4.2    P4.1    P4.0
                                //初始值=1111,1111      1       1       1       1       1       1       1       1

sbit P47        =   P4^7;       //I/O口P4.7
sbit P46        =   P4^6;       //I/O口P4.6
sbit P45        =   P4^5;       //I/O口P4.5
sbit P44        =   P4^4;       //I/O口P4.4
sbit P43        =   P4^3;       //I/O口P4.3
sbit P42        =   P4^2;       //I/O口P4.2
sbit P41        =   P4^1;       //I/O口P4.1
sbit P40        =   P4^0;       //I/O口P4.0

sbit RST        =   P4^7;       //复位脚,可通过编程器设置
sbit EX_LVD     =   P4^6;       //外部低压检测引脚

sbit P4SPISCLK  =   P4^3;       //SPI总线的时钟脚
sbit P4SPIMISO  =   P4^2;       //SPI总线的主出从入脚
sbit P4SPIMOSI  =   P4^1;       //SPI总线的主入从出脚
sbit P4SPISS    =   P4^0;       //SPI总线的从机选择脚

sbit P4CEX1     =   P4^3;       //比较/捕获/PWM1外部引脚
sbit P4CEX0     =   P4^2;       //比较/捕获/PWM0外部引脚
sbit P4ECI      =   P4^1;       //比较/捕获/PWM模块的外部时钟输入脚(最大值为Fosc/2)

sbit P4TXD2     =   P4^3;       //串口2数据输出脚
sbit P4RXD2     =   P4^2;       //串口2数据输入脚

/////////////////////////////////

sfr  P5         =   0xc8;       //I/O端口5              Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       -       -       -       P5.3    P5.2    P5.1    P5.0
                                //初始值=xxxx,1111      x       x       x       x       1       1       1       1

sbit P53        =   P5^3;       //I/O口P5.3
sbit P52        =   P5^2;       //I/O口P5.2
sbit P51        =   P5^1;       //I/O口P5.1
sbit P50        =   P5^0;       //I/O口P5.0

/////////////////////////////////

sfr  P1M1       =   0x91;       //I/O口1模式控制器1     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初/始值=0000,0000     0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P1M0       =   0x92;       //I/O口1模式控制器0     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P0M1       =   0x93;       //I/O口0模式控制器1     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P0M0       =   0x94;       //I/O口0模式控制器0     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P2M1       =   0x95;       //I/O口2模式控制器1     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P2M0       =   0x96;       //I/O口2模式控制器0     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P3M1       =   0xb1;       //I/O口3模式控制器1     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初/始值=0000,0000     0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P3M0       =   0xb2;       //I/O口3模式控制器0     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P4M1       =   0xb3;       //I/O口4模式控制器1     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P4M0       =   0xb4;       //I/O口4模式控制器0     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  P5M1       =   0xc9;       //I/O口5模式控制器1     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=xxxx,0000      x       x       x       x       0       0       0       0

/////////////////////////////////

sfr  P5M0       =   0xca;       //I/O口5模式控制器0     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=xxxx,0000      x       x       x       x       0       0       0       0

/////////////////////////////////

// PxM1 PxM0
//  0    0     准双向口
//  0    1     强推挽输出
//  1    0     高阻输入态
//  1    1     开漏输出

/////////////////////////////////

sfr  P1ASF      =   0x9d;       //P1口模拟功能控制位    Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                P17ASF  P16ASF  P15ASF  P14ASF  P13ASF  P12ASF  P11ASF  P10ASF
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

#define P17ASF      0x80        //P1.7口为ADC输入脚
#define P16ASF      0x40        //P1.7口为ADC输入脚
#define P15ASF      0x20        //P1.7口为ADC输入脚
#define P14ASF      0x10        //P1.7口为ADC输入脚
#define P13ASF      0x08        //P1.7口为ADC输入脚
#define P12ASF      0x04        //P1.7口为ADC输入脚
#define P11ASF      0x02        //P1.7口为ADC输入脚
#define P10ASF      0x01        //P1.7口为ADC输入脚

/////////////////////////////////

sfr  P4SW       =   0xbb;       //P4口的功能开关        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -    LVD_P4.6 ALE_P4.5 NA_P4.4  -       -       -       -
                                //初始值=x000,xxxx      x       0       0       0       x       x       x       x

#define P46EN       0x40        //传统的EA脚 0:为LVD的信号引入脚 1:为P4.6
#define P45EN       0x20        //传统的ALE脚 0:仍为ALE 1:为P4.5
#define P44EN       0x10        //传统的PSEN脚 0:无任何功能 1:为P4.4

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  IE         =   0xa8;       //中断使能寄存器        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                EA      ELVD    EADC    ES      ET1     EX1     ET0     EX0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

sbit EA         =   IE^7;       //总中断开关
sbit ELVD       =   IE^6;       //LVD中断使能位
sbit EADC       =   IE^5;       //ADC中断使能位
sbit ES         =   IE^4;       //串口中断使能位
sbit ET1        =   IE^3;       //定时器1中断使能位
sbit EX1        =   IE^2;       //外部中断1中断使能位
sbit ET0        =   IE^1;       //定时器0中断使能位
sbit EX0        =   IE^0;       //外部中断0中断使能位

/////////////////////////////////

sfr  IE2        =   0xaf;       //中断使能寄存器2       Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       -       -       -       -       -       ESPI    ES2
                                //初始值=xxxx,xx00      x       x       x       x       x       x       0       0

#define ESPI        0x02        //SPI中断使能位
#define ES2         0x01        //串口2中断使能位

/////////////////////////////////

#define X0_INTNO    0           //外部中断0中断号,入口地址为0003H
#define T0_INTNO    1           //定时器0中断号,入口地址为000BH
#define X1_INTNO    2           //外部中断1中断号,入口地址为0013H
#define T1_INTNO    3           //定时器1中断号,入口地址为001BH
#define UART_INTNO  4           //串口中断号,入口地址为0023H
#define ADC_INTNO   5           //ADC中断号,入口地址为002BH
#define LVD_INTNO   6           //LVD中断号,入口地址为0033H
#define UART2_INTNO 8           //串口2中断号,入口地址为0043H
#define SPI_INTNO   9           //SPI中断号,入口地址为004BH

/////////////////////////////////

sfr  IP2        =   0xb5;       //中断优先级寄存器2     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       -       -       -       -       -       PSPI    PS2
                                //初始值=xxxx,xx00      x       x       x       x       x       x       0       0

#define PSPI        0x02        //SPI中断优先级控制低位
#define PS2         0x01        //串口2中断优先级控制低位

/////////////////////////////////

sfr  IP2H       =   0xb6;       //中断优先级寄存器2高位 Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       -       -       -       -       -       PSPIH   PS2H
                                //初始值=xxxx,xx00      x       x       x       x       x       x       0       0

#define PSPIH       0x02        //SPI中断优先级控制高位
#define PS2H        0x01        //串口2中断优先级控制高位

/////////////////////////////////

sfr  IPH        =   0xb7;       //中断优先级寄存器高位  Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                PPCAH   PLVDH   PADCH   PSH     PT1H    PX1H    PT0H    PX0H
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

#define PPCAH       0x80        //PCA中断优先级控制高位
#define PLVDH       0x40        //LVD中断优先级控制高位
#define PADCH       0x20        //ADC中断优先级控制高位
#define PSH         0x10        //串口中断优先级控制高位
#define PT1H        0x08        //定时器1中断优先级控制高位
#define PX1H        0x04        //外部中断1中断优先级控制高位
#define PT0H        0x02        //定时器0中断优先级控制高位
#define PX0H        0x01        //外部中断0中断优先级控制高位

/////////////////////////////////

sfr  IP         =   0xb8;       //中断优先级寄存器      Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                PPCA    PLVD    PADC    PS      PT1     PX1     PT0     PX0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

sbit PPCA       =   IP^7;       //PCA中断优先级控制低位
sbit PLVD       =   IP^6;       //LVD中断优先级控制低位
sbit PADC       =   IP^5;       //ADC中断优先级控制低位
sbit PS         =   IP^4;       //串口中断优先级控制低位
sbit PT1        =   IP^3;       //定时器1中断优先级控制低位
sbit PX1        =   IP^2;       //外部中断1中断优先级控制低位
sbit PT0        =   IP^1;       //定时器0中断优先级控制低位
sbit PX0        =   IP^0;       //外部中断0中断优先级控制低位

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  TCON       =   0x88;       //定时器0/1控制寄存器   Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                TF1     TR1     TF0     TR0     IE1     IT1     IE0     IT0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

sbit TF1        = TCON^7;       //定时器1溢出标志,由硬件置1,可由软件清0或者在中断处理程序完成后由硬件自动清0
sbit TR1        = TCON^6;       //定时器1的启动控制位,1:启动定时器 0:停止定时器
sbit TF0        = TCON^5;       //定时器0溢出标志,由硬件置1,可由软件清0或者在中断处理程序完成后由硬件自动清0
sbit TR0        = TCON^4;       //定时器0的启动控制位,1:启动定时器 0:停止定时器
sbit IE1        = TCON^3;       //外部中断1的中断请求位,由硬件置1,可由软件清0或者在中断处理程序完成后由硬件自动清0
sbit IT1        = TCON^2;       //外部中断1的触发类型,0:低电平触发外部中断 1:下降沿触发外部中断
sbit IE0        = TCON^1;       //外部中断0的中断请求位,由硬件置1,可由软件清0或者在中断处理程序完成后由硬件自动清0
sbit IT0        = TCON^0;       //外部中断0的触发类型,0:低电平触发外部中断 1:下降沿触发外部中断

/////////////////////////////////

sfr  TMOD       =   0x89;       //定时器0/1模式寄存器   Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                GATE    C/T#    M1      M0      GATE    C/T#    M1      M0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

#define GATE1       0x80        //定时器1启动控制位,0:TR1为1时启动定时器1 1:TR1为1且INT1为高电平时才启动定时器1
#define C_T1        0x40        //定时器/计数器选择位,0:定时器(时钟源为内部时钟) 1:计数器(时钟源为T1引脚的外部时钟)
#define T1_M0       0x00        //定时器1操作模式0:13位定时器
#define T1_M1       0x10        //定时器1操作模式1:16位定时器
#define T1_M2       0x20        //定时器1操作模式2:8位自动重载定时器
#define T1_M3       0x30        //定时器1操作模式3:定时器1停止
#define GATE0       0x08        //定时器1启动控制位,0:TR0为1时启动定时器1 1:TR0为1且INT0为高电平时才启动定时器0
#define C_T0        0x04        //定时器/计数器选择位,0:定时器(时钟源为内部时钟) 1:计数器(时钟源为T0引脚的外部时钟)
#define T0_M0       0x00        //定时器0操作模式0:13位定时器
#define T0_M1       0x01        //定时器0操作模式1:16位定时器
#define T0_M2       0x02        //定时器0操作模式2:8位自动重载定时器
#define T0_M3       0x03        //定时器0操作模式3:TL0和TH0为两组独立8位定时器

/////////////////////////////////

sfr  TL0        = 0x8a;         //定时器0的低8位计数值  Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  TL1        = 0x8b;         //定时器1的低8位计数值  Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  TH0        = 0x8c;         //定时器0的高8位计数值  Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  TH1        = 0x8d;         //定时器1的高8位计数值  Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  SCON       =   0x98;       //串口控制寄存器        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                SM0/FE  SM1     SM2     REN     TB8     RB8     TI      Ri
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

sbit FE         =   SCON^7;     //帧错误检测位,当串口模块在接收数据时没有检测到正确的停止位时,FE被硬件置1,需要由软件来清0(当SMOD0为1时,FE有效)
sbit SM0        =   SCON^7;     //串口模式设置位
sbit SM1        =   SCON^6;     //SM0/SM1=0/0:移位寄存器 0/1:8位可变波特率 1/0:9位固定波特率 1/1:9位可变波特率
sbit SM2        =   SCON^5;     //自动地址识别使能位
sbit REN        =   SCON^4;     //使能串口接收模块
sbit TB8        =   SCON^3;     //发送的第9位数据
sbit RB8        =   SCON^2;     //接收的第9位数据
sbit TI         =   SCON^1;     //发送完成中断标志
sbit RI         =   SCON^0;     //接收完成中断标志

/////////////////////////////////

sfr  SBUF       =   0x99;       //串口接收/发送数据     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=xxxx,xxxx      x       x       x       x       x       x       x       x

/////////////////////////////////

sfr  S2CON      =   0x9a;       //串口2控制寄存器       Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                SM0/FE  SM1     SM2     REN     TB8     RB8     TI      Ri
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

#define S2SM0       0x80        //串口2模式设置位
#define S2SM1       0x40        //SM0/SM1=0/0:移位寄存器 0/1:8位可变波特率 1/0:9位固定波特率 1/1:9位可变波特率
#define S2SM2       0x20        //自动地址识别使能位
#define S2REN       0x10        //使能串口2接收模块
#define S2TB8       0x08        //发送的第9位数据
#define S2RB8       0x04        //接收的第9位数据
#define S2TI        0x02        //发送完成中断标志
#define S2RI        0x01        //接收完成中断标志

/////////////////////////////////

sfr  S2BUF      =   0x9b;       //串口2接收/发送数据    Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=xxxx,xxxx      x       x       x       x       x       x       x       x

/////////////////////////////////

sfr  SADDR      =   0xa9;       //串口设备从地址寄存器  Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  SADEN      =   0xb9;       //串口设备从地址屏蔽位  Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  BRT        =   0x9c;       //独立波特率发生器重载值Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  ADC_CONTR  =   0xbc;       //ADC控制寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述            ADC_POWER  SPEED1 SPEED0 ADC_FLAG ADC_START CHS2    CHS1    CHS0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

#define ADC_POWER   0x80        //ADC模块电源控制位
#define ADC_SPEEDLL 0x00        //每次转换需要420个时钟周期
#define ADC_SPEEDL  0x20        //每次转换需要280个时钟周期
#define ADC_SPEEDH  0x40        //每次转换需要140个时钟周期
#define ADC_SPEEDHH 0x60        //每次转换需要70个时钟周期
#define ADC_FLAG    0x10        //ADC转换完成标志
#define ADC_START   0x08        //ADC开始转换控制位
#define ADC_CHS2    0x04        //ADC通道选择位2
#define ADC_CHS1    0x02        //ADC通道选择位1
#define ADC_CHS0    0x01        //ADC通道选择位0

/////////////////////////////////

sfr  ADC_RES    =   0xbd;       //ADC结果高字节         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  ADC_RESL   =   0xbe;       //ADC结果低字节         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  IAP_DATA   =   0xc2;       //IAP数据寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=1111,1111      1       1       1       1       1       1       1       1

/////////////////////////////////

sfr  IAP_ADDRH  =   0xc3;       //IAP地址高位寄存器     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  IAP_ADDRL  =   0xc4;       //IAP地址低位寄存器     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  IAP_CMD    =   0xc5;       //IAP命令寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=xxxx,xx00      x       x       x       x       x       x       0       0

#define IAP_IDLE    0x00        //IAP待机命令
#define IAP_READ    0x01        //IAP读字节数据命令
#define IAP_PROGRAM 0x02        //IAP字节编程命令
#define IAP_ERASE   0x03        //IAP扇区擦除命令(每扇区512字节))

/////////////////////////////////

sfr  IAP_TRIG   =   0xc6;       //IAP命令触发寄存器     Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=xxxx,xxxx      x       x       x       x       x       x       x       x

#define IAP_TRIG0   0x5a        //IAP功能触发命令1
#define IAP_TRIG1   0xa5        //IAP功能触发命令2

/////////////////////////////////

sfr  IAP_CONTR  =   0xc7;       //IAP控制寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                IAPEN   SWBS    SWRST   CMD_FAIL -      WT2     WT1     WT0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

#define IAPEN       0x80        //IAP使能开关
#define SWBS        0x40        //软启动选择,0:从用户AP启动 1:从ISP启动
#define SWRST       0x20        //触发软件复位
#define CMD_FAIL    0x10        //IAP命令失败标志

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  SPSTAT     =   0xcd;       //SPI状态寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                SPIF    WCOL    -       -       -       -       -       -
                                //初始值=11xx,xxxx      1       1       x       x       x       x       x       x

#define SPIF        0x80        //SPI传输完成标志,当SPI中断使能时会触发SPI中断请求,需要用软件向此位写"1"来清0
#define WCOL        0x40        //SPI写冲突标志,需要用软件向此位写"1"来清0

/////////////////////////////////

sfr  SPCTL      =   0xce;       //SPI控制寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                SSIG    SPEN    DORD    MSTR    CPOL    CPHA    SPR1    SPR0
                                //初始值=0000,0100      0       0       0       0       0       1       0       0

#define SSIG        0x80        //SS忽略位,1:由MSTR确定主/从 0:由SS脚确定主/从
#define SPEN        0x40        //SPI模块使能位
#define DORD        0x20        //SPI数据顺序,1:LSB(先收/发最低位) 0:MSB(先收/发最高位)
#define MSTR        0x10        //主/从模式选择位
#define CPOL        0x08        //SPI时钟极性
#define CPHA        0x04        //SPI相位选择
#define SPSPEEDHH   0x00        //最快的速度,CPU_CLK/4
#define SPSPEEDH    0x01        //最快的速度,CPU_CLK/16
#define SPSPEEDL    0x02        //最快的速度,CPU_CLK/64
#define SPSPEEDLL   0x03        //最快的速度,CPU_CLK/128

/////////////////////////////////

sfr  SPDAT      =   0xcf;       //SPI数据寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sfr  CCON       =   0xd8;       //PCA控制寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                CF      CR      -       -       -       -       CCF1    CCF0
                                //初始值=00xx,xx00      0       0       x       x       x       x       0       0

sbit CF         =   CCON^7;     //PCA计数器溢出标志,当PCA中断使能时会触发PCA中断请求,需要用软件清0
sbit CR         =   CCON^6;     //PCA计数器启动控制位,写"1"启动东计数器
sbit CCF1       =   CCON^1;     //PCA模块1中断标志,需要用软件清0
sbit CCF0       =   CCON^0;     //PCA模块0中断标志,需要用软件清0

/////////////////////////////////

sfr  CMOD       =   0xd9;       //PCA模式寄存器         Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                CIDL    -       -       -       CPS2    CPS1    CPS0    ECF
                                //初始值=0xxx,0000      0       x       x       x       0       0       0       0

#define CIDL        0x80        //计时器阵列空闲控制,0:空闲模式下PCA继续工作 1:空闲模式下PCA停止工作
#define PCACLK0     0x00        //Fosc/12
#define PCACLK1     0x02        //Fosc/2
#define PCACLK2     0x04        //定时器0的溢出率
#define PCACLK3     0x06        //ECI外部时钟
#define PCACLK4     0x08        //Fosc/1
#define PCACLK5     0x0a        //Fosc/4
#define PCACLK6     0x0c        //Fosc/6
#define PCACLK7     0x0e        //Fosc/8
#define ECF         0x01        //PCA计数器溢出中断允许位

/////////////////////////////////

sfr  CCAPM0     =   0xda;       //PCA模块0模式寄存器    Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       ECOM0   CAPP0   CAPN0   MAT0    TOG0    PWM0    ECCF0
                                //初始值=x000,0000      x       0       0       0       0       0       0       0

#define PCA0_ECOM   0x40        //使能比较器0
#define PCA0_CAPP   0x20        //使能PCA模块0的上升沿捕获
#define PCA0_CAPN   0x10        //使能PCA模块0的下降沿捕获
#define PCA0_MAT    0x08        //匹配使能,当PCA的计数值与模块0设定的值相匹配时,置CCON.CCF0
#define PCA0_TOG    0x04        //翻转控制位,当PCA的计数值与模块0设定的值相匹配时,翻转CEX0的电平
#define PCA0_PWM    0x02        //脉宽调制模式,使能CEX0为脉宽调制输出口
#define PCA0_ECCF   0x01        //使能CCF0中断

/////////////////////////////////

sfr  CCAPM1     =   0xdb;       //PCA模块1模式寄存器    Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       ECOM1   CAPP1   CAPN1   MAT1    TOG1    PWM1    ECCF1
                                //初始值=x000,0000      x       0       0       0       0       0       0       0

#define PCA1_ECOM   0x40        //使能比较器1
#define PCA1_CAPP   0x20        //使能PCA模块1的上升沿捕获
#define PCA1_CAPN   0x10        //使能PCA模块1的下降沿捕获
#define PCA1_MAT    0x08        //匹配使能,当PCA的计数值与模块1设定的值相匹配时,置CCON.CCF1
#define PCA1_TOG    0x04        //翻转控制位,当PCA的计数值与模块1设定的值相匹配时,翻转CEX1的电平
#define PCA1_PWM    0x02        //脉宽调制模式,使能CEX1为脉宽调制输出口
#define PCA1_ECCF   0x01        //使能CCF1中断

/////////////////////////////////

//CCAPM0   -   ECOM CAPP CAPN MAT  TOG  PWM  CCF
// 00H     0    0    0    0    0    0    0    0     无操作,待机状态
// 42H     0    1    0    0    0    0    1    0     8位PWM输出,无中断
// 63H     0    1    1    0    0    0    1    1     8位PWM输出,产生CEX0上升沿中断
// 53H     0    1    0    1    0    0    1    1     8位PWM输出,产生CEX0下降沿中断
// 73H     0    1    1    1    0    0    1    1     8位PWM输出,产生CEX0跳变中断
// 20H     0    x    1    0    0    0    0    0     16位捕获模式,CEX0上升沿触发捕获
// 10H     0    x    0    1    0    0    0    0     16位捕获模式,CEX0下降沿触发捕获
// 30H     0    x    1    1    0    0    0    0     16位捕获模式,CEX0跳变触发捕获
// 48H     0    1    0    0    1    0    0    0     16位软件定时器
// 4CH     0    1    0    0    1    1    0    0     16位高速脉冲输出

/////////////////////////////////

sfr  CL         =   0xe9;       //PCA计数器低8位        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  CH         =   0xf9;       //PCA计数器高8位        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  CCAP0L     =   0xea;       //PCA模块0寄存器低8位   Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  CCAP1L     =   0xeb;       //PCA模块1寄存器低8位   Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  CCAP0H     =   0xfa;       //PCA模块0寄存器高8位   Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  CCAP1H     =   0xfb;       //PCA模块1寄存器高8位   Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //初始值=0000,0000      0       0       0       0       0       0       0       0

/////////////////////////////////

sfr  PCA_PWM0   =   0xf2;       //PCA0模式PWM辅助寄存器 Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       -       -       -       -       -       EPC0H   EPC0L
                                //初始值=xxxx,xx00      x       x       x       x       x       x       0       0

/////////////////////////////////

sfr  PCA_PWM1   =   0xf3;       //PCA1模式PWM辅助寄存器 Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
                                //位描述                -       -       -       -       -       -       EPC1H   EPC1L
                                //初始值=xxxx,xx00      x       x       x       x       x       x       0       0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
