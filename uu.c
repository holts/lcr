//==========================================================================
//  ESR表驱动程序 V2.0
//  许剑伟 于莆田 2012.10
//==========================================================================
//==========================================================================
#define uchar unsigned char
#define uint  unsigned int
#define ulong  unsigned long
#include <reg52.h>

//==========================================================================
// 项目：LCD1602 四线驱动程序
// 设计要点：
//     LCD1602 的运行速度慢，而单片机运行的速度快，因此容易因为速度不
//     匹配造成调试失败。因此，调试之前应准确测试lcd_delay() 延时函数
//     准确的延时量，如果不能满足注释中的要求，则应调整循次数。每步操
//     作所需的延时量，按照数据手册指标指行，同时留下足够的时间余量。
// 硬件连接：
//     至少需要9条线，电源线2条，7条信号线。信号线详见程序中的接口定义。
//     清注意对LCD1602比对的调节，否则无显示。
// 设计：许剑伟,于莆田,2010.12
//==========================================================================
sbit lcd_RS = P0^5; //数据命令控制位,0命令1数据
sbit lcd_RW = P0^6; //读写位,0写1读
sbit lcd_EN = P0^4; //使能位,下降沿触发
sbit lcd_D4 = P0^3; //数据端口D4
sbit lcd_D5 = P0^2; //数据端口D5
sbit lcd_D6 = P0^1; //数据端口D6
sbit lcd_D7 = P0^0; //数据端口D7
//==========================================================================
void lcd_delay(int n){ //LCD专用延时函数
  //32MHz钟频下，约循环3000次延迟1毫秒
  int i,j;
  if(n<0)    { for(i=0;i< 20;i++); return; } //10us
  if(n== 0)  { for(i=0;i<100;i++); return; } //50us
  for(;n;n--){ for(j=0;j<2000;j++);        } //n毫秒
}
//==========================================================================
void lcd_B(char f, uchar c, char t){ //控制四线式接口LCD的7个脚
  //f=0写命令字, f=1写RAM数据, f=2读地址（或读忙）, f=3读RAM数据
  lcd_EN = 0;
  lcd_RS = f%2;
  lcd_RW = f/2%2;
  //移入高四位
  lcd_D4 = c & 16;
  lcd_D5 = c & 32;
  lcd_D6 = c & 64;
  lcd_D7 = c & 128;
  lcd_EN = 1;  lcd_delay(-1);  lcd_EN = 0; //使能脉冲
  if(f==4) { lcd_delay(t); return; }
  //移入低四位
  lcd_D4 = c & 1;
  lcd_D5 = c & 2;
  lcd_D6 = c & 4;
  lcd_D7 = c & 8;
  lcd_EN = 1;  lcd_delay(-1);  lcd_EN = 0; //使能脉冲
  lcd_delay(t);  //不同的命令,响应时间不同,清零命令需要2ms
}
//==========================================================================
void lcd_init(){ //LCD1602 初始化
  //启动四线模式须势行9个步骤，初始化所须耗时较长，约65ms，时限不可减
  lcd_delay(20); //启动lcd之前须延时大于15ms，直到VDD大于4.5V
  lcd_B(4, 0x30, 9); //置8线模式,须延时大于4.1ms
  lcd_B(4, 0x30, 5); //置8线模式,须延时大于100us
  lcd_B(4, 0x30, 5); //置8线模式,手册中未指定延时
  lcd_B(4, 0x20, 5); //进入四线模式
  lcd_B(0, 0x28, 5); //四线模式双行显示
  lcd_B(0, 0x0C, 5); //打开显示器
  lcd_B(0, 0x80, 5); //RAM指针定位
  lcd_B(0, 0x01, 5); //启动清屏命初始化LCD
}
//==========================================================================
//=========================几个功能常用函数=================================
void lcd_cls()         { lcd_B(0, 0x01+0, 2);  } //清屏
void lcd_cur0()        { lcd_B(0, 0x0C+0, 0);  } //隐藏光标
void lcd_goto1(uchar x){ lcd_B(0, 0x80+x, 0);  } //设置DDRAM地址,第1行x位
void lcd_goto2(uchar x){ lcd_B(0, 0xC0+x, 0);  } //设置DDRAM地址,第2行x位
void lcd_putc(uchar d) { lcd_B(1, 0x00+d, 0);  } //字符输出
void lcd_puts(uchar *s){ for(; *s; s++) lcd_B(1,*s,0); } //字串输出
//==============字符显示函数====================
#define digW 4 //数字显示位数宏
void lcd_puti(int a,char w){ //定宽显示正整数
  char i=0, s[5] = {' ',' ',' ',' ',' '};
  if(a<0) { a=-a; lcd_puts("-"); }
  else    lcd_puts(" ");
  do{
   s[i++] = a%10+48;
   a /= 10;
  }while(a);
  for(;w;w--) lcd_putc(s[w-1]);
}
void lcd_putf(float a,char n,char w){ //浮点输出，n是保留小数的位数,w是数字宽度
 char i,g,fi=0;
 long b,c=1;
 if(a<0) { lcd_putc('-'); a = -a; }
 else    { lcd_putc(' '); }
 for(i=0;i<n;i++) a *= 10;
 for(i=1;i<w;i++) c *= 10;
 b = a;
 for(i=0;i<w;i++){
  g = b/c;
  b -= g*c;
  c /= 10;
  if(g>9||g<0) g='*'-48;
  if(i == w-n ) lcd_putc('.');
  if(!g && !fi && i<w-n-1) { lcd_putc(' '); continue; }
  lcd_putc(g+48);
  fi = 1;
 }
 if(!n) lcd_putc(' '); //无小数点的补足显示宽度
}

//==========================================================================
//===============================延时函数===================================
void delay(uint loop) { uint i; for(i=0;i<loop;i++); } //延时函数
void delay2(uint k)   { for(;k>0;k--) delay(10000);  } //长延时,k=100大约对应1秒

//==========================================================================
//=================================AD转换===================================
sfr P1ASF = 0x9D;     //将P1置为模拟口寄存器(使能),各位中为1的有效
sfr ADC_CONTR = 0xBC; //A/D转换控制寄存器
sfr ADC_res   = 0xBD; //A/D转换结果寄存器
sfr ADC_resl  = 0xBE; //A/D转换结果寄存器

void set_channel(char channel){
 P1ASF = 1<<channel;
 ADC_CONTR = channel+128; //最高位是电源开关,低3位通道选择
 delay(1); //首次打开电源应延迟，使输入稳定
}
uint getAD2(){
 ADC_CONTR |= 0x08;             //00001000,置ADC_START=1启动A/D 转换
 while ( !(ADC_CONTR & 0x10) ); //等待A/D转换结束(ADC_FLAG==0)
 ADC_CONTR &= 0xE7;             //11100111,置ADC_FLAG=0清除结束标记, 置ADC_START=0关闭A/D 转换
 return ADC_res*4 + ADC_resl;
}
/*
uchar get_AD(){
 ADC_CONTR |= 0x08;             //00001000,置ADC_START=1启动A/D 转换
 while( !(ADC_CONTR & 0x10) );  //等待A/D转换结束(ADC_FLAG==0)
 ADC_CONTR &= 0xE7;             //11100111,置ADC_FLAG=0清除结束标记, 置ADC_START=0关闭A/D 转换
 return ADC_res;
}
*/


//==========================================================================
//==================================EEPROW偏程==============================
sfr IAP_data  = 0xC2;
sfr IAP_addrH = 0xC3;
sfr IAP_addrL = 0xC4;
sfr IAP_cmd   = 0xC5;
sfr IAP_trig  = 0xC6;
sfr IAP_contr = 0xC7;
/********************
写字节时，可以将原有数据中的1改为0，无法将0改为1，只能使用擦除命令将0改为1
应注意，擦除命令会将整个扇区擦除
*********************/
int eepEn = 0;
void saEEP(){ //触发并EEP保护
 if(eepEn==12345) IAP_trig = 0x5A;  //先送5A
 if(eepEn==12345) IAP_trig = 0xA5;  //先送5A再送A5立即触发
 IAP_cmd = 0;      //关闭令，保护
 IAP_contr = 0;    //关EEPROM，保护
 IAP_trig = 0;
 IAP_addrL = 255; //设置读取地址的低字节，地址改变才需要设置
 IAP_addrH = 255; //设置读取地址的高字节，地址改变才需要设置
}
uchar readEEP(uint k){ //读取
 IAP_addrL = k;    //设置读取地址的低字节，地址改变才需要设置
 IAP_addrH = k>>8; //设置读取地址的高字节，地址改变才需要设置
 IAP_contr = 0x81; //设置等待时间，1MHz以下取7，2M以下取6，3M取5，6M取4，12M取3，20M取2，24M取1，30M取0，前导1表示许档IAP
 IAP_cmd = 1;      //读取值1，写取2，擦除取3，擦除时按所在字节整个扇区撺除
 saEEP(); //触发并保护
 return IAP_data;
}
void writeEEP(uint k, uchar da){ //写入
 IAP_data = da;    //传入数据
 IAP_addrL = k;    //设置读取地址的低字节，地址改变才需要设置
 IAP_addrH = k>>8; //设置读取地址的高字节，地址改变才需要设置
 IAP_contr = 0x81; //设置等待时间，1MHz以下取7，2M以下取6，3M取5，6M取4，12M取3，20M取2，24M取1，30M取0，前导1表示许档IAP
 IAP_cmd = 2;      //读取值1，写取2，擦除取3，擦除时按所在字节整个扇区撺除
 saEEP(); //触发并保护
}
void eraseEEP(uint k){ //擦除
 IAP_addrL = k;    //设置读取地址的低字节，地址改变才需要设置
 IAP_addrH = k>>8; //设置读取地址的高字节，地址改变才需要设置
 IAP_contr = 0x81; //设置等待时间，1MHz以下取7，2M以下取6，3M取5，6M取4，12M取3，20M取2，24M取1，30M取0，前导1表示许档IAP
 IAP_cmd = 3;      //读取值1，写取2，擦除取3，擦除时按所在字节整个扇区撺除
 saEEP(); //触发并保护
}


xdata struct Ida{
 int x0;  //清零时刻的72kHz零点，0度
 int y0;  //清零时刻的72kHz零点，270度
 int z0;  //清零时刻的6kHz零点，270度
 int d0;  //清零时刻的前端直流偏移
 char Vp; //幅度校准项
 char J0; //相位校准项
 char J1; //相位校准项2
 char Cm; //大电容校准
 int a0;  //72k,90度清零,L
 int b0;  //03k,90度清零,L
 int g0;  //直流零点,L
 int a1;  //20欧电阻的残余电抗,L内阻清零用
 int b1;  //20欧电阻的残余电抗,L内阻清零用
 int c1;  //直流电阻测量的AD零点偏移(测L内阻用)
} cs;

void cs_RW(char rw){
 uchar i,*p = &cs;
 const int offs=512;
 if(rw){
  eraseEEP(offs);
  for(i=0;i<sizeof(cs);i++) writeEEP(i+offs,p[i]);
 }else{
  for(i=0;i<sizeof(cs);i++) p[i]=readEEP(i+offs);
 }
}
//==============过采样加扰信号====================
//PCA相关寄存器
sfr CMOD = 0xD9;   //钟源选择控制等
sfr CH = 0xF9;     //PCA的计数器
sfr CL = 0xE9;     //PCA的计数器
sfr CCON = 0xD8;   //PCA控制寄存器
sfr CCPAM0 = 0xDA; //PCA模块0工作模式寄存器
sfr CCPAM1 = 0xDB; //PCA模块1工作模式寄存器
sfr CCAP0L = 0xEA; //模块0捕获寄存器低位
sfr CCAP0H = 0xFA; //模块0捕获寄存器高位

sbit PPCA  = IP^7;   //PCA的中断优先级设置
sbit CCF0  = CCON^0; //PCA的模块0中断标志
sbit CCF1  = CCON^1; //PCA的模块1中断标志
sbit CR = CCON^6;    //PCA计数器使能

void PWM_init(){ //把PCA置为PWM
  CMOD = 0;   //0000 0010 计数源选择,钟源取fosc/2, 0000 0000是fosc
  CL = CH = 0;
  CCAP0L = CCAP0H = 128; //占空比为50%
  CCPAM0=0x42;//0100 0010,PCA的模块0设置为PWM模式,无中断
  //CCPAM0=0x53;//0101 0011,PCA的模块0设置为PWM模式,有中断，下降沿中断
  //PPCA = 1;   //优先中断
  CR = 1;   //开始计数
  //EA = 1;     //开总中断
}


//==========================================================================
//==================================ESR主程序===============================
//==========================================================================
sfr P1M1=0x91; //P1端口设置寄存器
sfr P1M0=0x92; //P1端口设置寄存器
sfr P0M1=0x93; //P0端口设置寄存器
sfr P0M0=0x94; //P0端口设置寄存器
sfr P2M1=0x95; //P2端口设置寄存器
sfr P2M0=0x96; //P2端口设置寄存器
sfr P3M1=0xB1; //P3端口设置寄存器
sfr P3M0=0xB2; //P3端口设置寄存器
sfr WAKE_CLKO = 0x8F;
sfr AUXR = 0x8E;
sfr BRT = 0x9C;


sbit pt0=P3^4;  //方波信号输出
sbit pt1=P3^5;  //鉴相信号控制
sbit DCo=P1^6;  //直流信号输出
sbit pwk=P2^0;  //电源开关口
sbit pwK=P2^1;  //电源开关口2
xdata uchar menu=0,menu2=0; //菜单变量



//==============ESR测量====================
ulong nn = 0; //时序计数器
uchar nn2 = 0;

//=====可编程时钟发生器======
//每完成一趟计数，TL0、TL1重装，输出口置反，所以两趟计数才能输了完整方波，共计数2*(256-THx)字
//THx取128，输出方波需2*(256-128)=256计数，即256个时钟周期
uchar XW=0;
void feqInit(){
  WAKE_CLKO |= 0x07; //允许T0 T1 BRT独立发生时钟,以启动运放电源
  TMOD = 0x22;  //T0和T1工作在8位自动重装
  AUXR |= 0x80; //T0工作在1T
  AUXR |= 0x40; //T1工作在1T
  AUXR |= 0x04; //独立波特率发生器工作在1T
  TH0 = 128;    //置T0频率字
  TH1 = 128;    //置T1频率字
  BRT = 156;    //置BTR频率
  TL0 = 0;      //置初相为0
  TL1 = 0;      //置初相为0(TR1启动延时4周期，所以补偿4字)
  AUXR |= 0x10; //启动BRT,P1.0口输出
  TCON |= 0x50; //启动T0和T1,T0和T1分别在P3.4和P3.5输出,同时置1启动.如果用TR0=1,TR1=1,则1T时TL1应补偿4字.
}
void setXW(uchar xw,uchar bc){ //xw为移相参数(360度对应256字),bc补偿字数(72kHz补4字)
  uint m = 256-TH0;            //半周期的计数值
  uint n = (m*xw+64)/128+bc;   //变换为计数偏移量,因TR1比TR0晚启动引入4周期延时，所以事先补偿4字
  XW = xw;
  TR0 = 0, TR1 = 0;     //暂停计数
  pt0 = 0, pt1 = n/m%2; //同相或反相处理
  TL0 = 0, TL1 = n % m; //置相位差
  TCON |= 0x50; //TR0与TR1同时置1启动T0和T1.如果用TR0=1,TR1=1;则1T时bc应补偿4字
}
void setFeq(char k,uchar w){ //设置频率,w是相位
  if(k==4){ //直流电阻测量使用
	P3M1 |= 0x10, P3M0 &= ~0x10; //P3.4置为高阻口
    P1M0 |= 0x40, P1M1 &= ~0x40; //P1.6置为推挽口
	TR0 = TR1 = 0; //停止计数器
    pt1 = 1; //P3.5开路
	DCo = 1; //开启直流源
	return;
  }
  //交流阻抗测量使用
  P1M1 |= 0x40, P1M0 &= ~0x40; //P1.6置为高阻口
  P3M0 |= 0x10, P3M1 &= ~0x10; //P3.4置为推挽口
  if(k==0){ AUXR |=  0x80;  AUXR |=  0x40;  TH0=128, TH1=128; } //1T模式, 置为双72kHz
  if(k==1){ AUXR &= ~0x80;  AUXR &= ~0x40;  TH0=0,   TH1=0;   } //12T模式,置为双3kHZ
  if(k==2){ AUXR |=  0x80;  AUXR |=  0x40;  TH0=128, TH1=0;   } //1T模式, 信号源72kHz，检波端36kHz
  if(k==3){ AUXR |=  0xC0;  TH0=128; TR0=1; TR1=pt1=0; return;} //1T模式, 信号源72kHz，检波端0Hz
  setXW(w,0);
}
uint getAD10() { //过采样
 //每秒取样速度为2.48万
 int i;
 long c = 0;
 for(i=0;i<500;i++) c += getAD2();
 c = (c+25)/50;
 return c;
}
void putCx(float C){ //显示电容，占用LCD屏7字符
  char xs = 0;
  float d=C;
  if(d<0) d = -d;
  if(d>=10000) { lcd_puts(" OL    "); return; }
  if(d<10000) xs = 0;
  if(d<1000)  xs = 1;
  if(d<100)   xs = 2;
  if(d<10)    xs = 3;
  lcd_putf(C,xs,4);
  lcd_puts("u");
}
void putRx(float R,char n,char *dw){ //输出浮点和单位,可用于显示电阻、电感
  lcd_putf(R,n,4);
  lcd_puts(dw);
}
void putNx(char *Lb,int a,char n){ //输出整数
  lcd_puts(Lb);
  lcd_puti(a,n);
}

int Vx,Vy,Vz,Vd; //Vd是零点直流漂移
float Vpp=52500,Vbat; //Vpp信号电压,Vbat电池电压
char Ha=0;
void ESRcalc(char jiao, char ha){
 const Bn = 3; //时序步数
 char n = nn%(Bn*8);
 int v,vx,vy,vz,vd;
 float R=0,C=0,C2=0,e;
 if(n%Bn) return;
 n /= Bn;
 v = getAD10();
 if(n==0){ //测得电源电压
   set_channel(2); delay(10); Vpp  = getAD10()*8.05*(1+0.001*cs.Vp); //读信号电压
   set_channel(7); delay(10); Vbat = getAD2()*0.00954;               //读电池电压
   set_channel(1);
 }
 if(ha==0){ //测阻容
   if(n==0){ Vd = v; setFeq(0,  0); return; } //测得零点直流,置为72kHz,0度
   if(n==1){ Vx = v; setFeq(0,192); return; } //测得x相电压,置72kHz,270度
   if(n==2){ Vy = v; setFeq(1,192); return; } //测得y相电压,置低频,270度
   if(n==3){ Vz = v; setFeq(2,  0);         } //低频测得y相电压,置为非相关频率 
   vd = Vd - cs.d0;
   vx = Vx - cs.x0 - vd;
   vy = Vy - cs.y0 - vd;
   vz = Vz - cs.z0 - vd + cs.Cm;
   e = 1.0*vy/Vpp; e = e*(1+e*e*1.7);
   C = 14.05/8/104/e;     //电容量公式 (T/8)*(A/vy)/R,因输出端直流内阻是260欧，直流负载20k，要修正0.9%
   v = vy*(1.333*e + 0.005*cs.J1*e - 0.001*cs.J0);//电阻修正参数,理论系数是1.333，实测采用1.38比较好
   R = 1.0*(vx-v)/(Vpp-vx)*104; //电阻计算
   if(Vz<9800){ //低频测容量
     e = vz/Vpp; e = e*(1+e*e*1.7);
     C2 = 14.05/8/104/e*24;
   }
   if(jiao==0){ //阻容显示
    if(Vx>9800||Vy>9800){ //主参数溢出
      lcd_goto1(0); lcd_puts("Overflow        ");
      lcd_goto2(0); lcd_puts("C<120n or R>18R ");
      return;
    }
    lcd_goto1(0); lcd_puts("R=");  putRx(R,2,"\xF4"); putRx(Vbat,2,"V");//输出电阻和电池电压
    lcd_goto2(0); lcd_puts("C=");  putCx(C);          putCx(C2);        //电容输出
   }
   if(jiao==1) { //过程显示
    lcd_goto1(0); putNx("D=",cs.d0,4); putNx(" d=",vd,5); //零点直流
    lcd_goto2(0); putNx("X=",cs.x0,4); putNx(" x=",vx,5); //显示Vx
   }
   if(jiao==2) { //过程显示
    lcd_goto1(0); putNx("Y=",cs.y0,4); putNx(" y=",vy,5); //显示Vy
    lcd_goto2(0); putNx("Z=",cs.z0,4); putNx(" z=",vz,5); //显示Vz
   }
   //校准显示输出
   if(jiao==4){ //大电容校准,清零采用72k，与3kHz相差较远，清零效果不好，故人工校准，有望改善。
    lcd_goto1(0); lcd_puts("C=");  putCx(C2);
    lcd_goto2(0); putNx("Cm=",cs.Cm,5);
   }
   if(jiao>=5) { lcd_goto1(0); lcd_puts("R=");  putRx(R,2,"\xF4"); } //输出电阻
   if(jiao==5) { lcd_goto2(0); putNx("Vp=",cs.Vp,5);  }  //Vp校准输出
   if(jiao==6) { lcd_goto2(0); putNx("J0=",cs.J0,5);  }  //J0校准输出
   if(jiao==7) { lcd_goto2(0); putNx("J1=",cs.J1,5);  }  //J1校准输出
 }
 if(ha==1){ //测电感
   if(n==0){ Vd = v; setFeq(4, 0); return; } //测得零点直流,置为72kHz,90度
   if(n==1){ Vx = v; setFeq(0,64); return; } //测直流电阻
   if(n==2){ Vy = v; setFeq(1,64); return; } //测得y相电压,置低频,90度
   if(n==3){ Vz = v; setFeq(3, 0); return; } //低频测得y相电压,置为非相关频率 
   vd = Vd - cs.g0;
   vy = Vy - cs.a0 - vd;
   vz = Vz - cs.b0 - vd;
   vx = Vx + cs.c1;  if(vx<0) vx = 0; vx = vx*2/3; //得到直流电阻,单位是0.01欧,实际上测得220R与电感直流电阻的并联,因为电感内阻小，220R可忽加略，用vx代替电感内阻

   vy -= cs.a1*(vx/2000.0); //cs.a1是20欧对应的电抗偏移,72kHz
   vz -= cs.b1*(vx/2000.0); //cs.b1是20欧对应的电抗偏移,3kHz

   e = 354.0/Vpp;
   if(jiao==0){ //显示电感量
    lcd_goto1(0); lcd_puts("72k L ="); if(Vy>9800) lcd_puts("Over     "); else putRx(e*vy*(1+0.00020*vx)     , 2,"uH "); //72kHz电感
    lcd_goto2(0); lcd_puts("03k L ="); if(Vz>9800) lcd_puts("Over     "); else putRx(e*vz*(1+0.00014*vx)*24.3, 0,"uH "); //低频测电感
   }
   if(jiao==1){ //过程显示
    lcd_goto1(0); lcd_puts("Ind    ");
	putNx("DCR=",vx,4);
    lcd_goto2(0); putNx("G=",cs.g0,4); putNx(" g=",vd,5); //零点直流
   }
   if(jiao==2){ //过程显示
    lcd_goto1(0); putNx("A=",cs.a0,4); putNx(" a=",vy,5); //显示Va
    lcd_goto2(0); putNx("B=",cs.b0,4); putNx(" b=",vz,5); //显示Vb
   }
 }
}

void shortCls(){ //短路清零
 if(Ha&&Vx>2500&&Vx<3000){ //电感测量的电阻残余清零,接入标准20R电阻后进行清零
  uchar i;
  cs.a1 = Vy-cs.a0, cs.b1 = Vz-cs.b0;
  lcd_cls(); lcd_puts("Measure AD zeor.");
  setFeq(4,0);
  DCo=0, cs.c1=0;
  for(i=0;i<200;i++){ //每循环约1ms时间
    DCo = ~DCo;	delay(100); //生成占空比为1:10的方波
    DCo = ~DCo;	delay(900);
	if(i>=190) cs.c1 += getAD2(); //读电压,仅在最后5%时间读电压，电容上的电压还有一定的保持作用，因此getAD2()占用的时间虽然影响占空比，但对读值准确性影响很小
  }
  DCo=1, cs.c1 = 275 - cs.c1; //每1欧150字,20//220=18.3,1/10即为1.83欧对应275字
  lcd_goto2(0); putNx("c1=",cs.c1,4);
  cs_RW(1); //保存
  lcd_goto2(0); lcd_puts("Ind cleared.");
  delay2(100);
  return;
 }
 if(Ha) cs.g0 = Vd, cs.a0 = Vy, cs.b0 = Vz;             //零、72kHz(90度)、3kHz(90度) 电感测量
 else   cs.d0 = Vd, cs.y0 = Vy, cs.z0 = Vz, cs.x0 = Vx; //零、72kHz(0度)、 72kHz(270度)、3kHz(270度) 阻容测量
 cs_RW(1); //保存
 lcd_cls();
 lcd_puts("Zero... OK");
 delay2(100);
 Ha = (Ha+1)%2;
}

main(){
 uchar kn=0,key=0,keya=0,keyb=0; //键盘响应变量
 pwk = 0; //打开电源
 //端口初始化
 P1M0 = 0x09;    //P1.0 P1.3置为推勉口
 P1M1 = 0x86;    //10000110 P1.1 P1.2 P1.7置为高阻抗
 P3M0 = 0x30;    //00110000 P3.4置为推勉口(T0输出),P3.5置为开漏
 P3M1 = 0x20;    //00100000
 P2M0 = 0x02;    //P2.1置为高阻口
 P2M1 = 0x02;    //P2.1置为高阻口

 feqInit();
 PWM_init();

 delay2(5);    //等待升压电源电压上升
 lcd_init();   //初始化LCD
 lcd_cur0();   lcd_puts("ESR V2.0");   //隐藏光标并显示片本
 lcd_goto2(0); lcd_puts("XJW Putian,2012"); //显示作者
 delay2(200);  lcd_cls(); //启动延时
 eepEn= 12345;
 cs_RW(0);   //读EEPROM

 set_channel(1); //设置AD转换通道

 while(1){
  nn++;
  if(nn%100==0) nn2++; //关机定时器,nn2每计数约3.0秒
  if(nn2==200) pwk = 1; //关电源
  if(pwK){ //关机键
    delay(20000); //等待按键稳定
    while(pwK);	lcd_cls();	lcd_puts("Power off ..."); //键弹起后显示准备关机信号
    for(kn=0;1;kn++){//延时几秒秒再关机，如果这期间power键重按，则不关机，而进行LC切换
	  if(kn==240) pwk = 1; //关机
	  lcd_goto2(0); lcd_puti(kn/20,3);
	  if(pwK) { kn=0; break; } //power重按则跳出
	  delay(20000);
	}
    delay(20000); //等待按键稳定
    while(pwK); Ha = (Ha+1)%2;  //键弹起后LC切换测量
    nn2 = 0; //有按键动作，关机定时器置0
  }
  //扫描键盘
  key=0;  keya=keyb;  keyb=(~P3)&(128+64+8);
  if(keya){
   if(!keyb){ //键弹起
     if(kn>2)  key = keya;   //短按
     if(kn>40) key = keya+1; //长按,约1至2秒
     kn = 0;
	 nn2 = 0; //有按键动作，关机定时器置0
   }
   else kn += kn<255?1:0;
  }
  //菜单系统
  if(key==128){ //菜单键
	lcd_cls();
	if(++menu>4) menu=0;
  }
  if(key==129){ //保存参数
   cs_RW(1);
   lcd_cls();
   lcd_puts("saved OK");
   delay2(100);
  }
  if(menu==0){ //显示ESR
    static char jia=0; //显示内容切换
    ESRcalc(jia,Ha);
	if(key==64) shortCls(); //短路清零
	if(key==8) jia++, jia %= 3;
  }
  if(menu==1){ //Vp校准
    ESRcalc(5,0);
    if(key==64) cs.Vp++;
    if(key==8)  cs.Vp--;
  }
  if(menu==2){ //J0校准
    ESRcalc(6,0);
    if(key==64) cs.J0++;
    if(key==8)  cs.J0--;
  }
  if(menu==3){ //J1校准
    ESRcalc(7,0);
    if(key==64) cs.J1++;
    if(key==8)  cs.J1--;
  }
  if(menu==4){ //Cm校准
    ESRcalc(4,0);
    if(key==64) cs.Cm++;
    if(key==8)  cs.Cm--;
  }
  delay(20000);
 }//while end
}
//==========================================================================

