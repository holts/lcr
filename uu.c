//==========================================================================
//  LCR表驱动程序 V2.0
//  许剑伟 于莆田 2012.01
//==========================================================================
//==========================================================================
#define uchar unsigned char
#define uint  unsigned int
#define ulong  unsigned long
#include <reg52.h>
#include <math.h>

//==========================================================================
// 项目：LCD1602 四线驱动程序
// 设计要点：
//     LCD1602 的运行速度慢，而单片机运行的速度快，因此容易因为速度不
//     匹配造成调试失败。因此，调试之前应准确测试lcd_delay() 延时函数
//     准确的延时量，如果不能满足注释中的要求，则应调整循次数。每步操
//     作所需的延时量，按照数据手册指标指行，同时留下足够的时间余量。
// 硬件连接：
//     至少需要9条线，电源线2条，7条信号线。信号线详见程序中的接口定义。
//     清注意对LCD1602对比度的调节，否则无显示。
// 设计：许剑伟,于莆田,2010.12
//==========================================================================
sbit lcd_RS = P0^6; //数据命令控制位,0命令1数据
sbit lcd_RW = P0^5; //读写位,0写1读
sbit lcd_EN = P0^4; //使能位,下降沿触发
sbit lcd_D4 = P0^3; //数据端口D4
sbit lcd_D5 = P0^2; //数据端口D5
sbit lcd_D6 = P0^1; //数据端口D6
sbit lcd_D7 = P0^0; //数据端口D7
//==========================================================================
code char path[40]={
0x00,0x00,0x0e,0x11,0x11,0x0a,0x1b,0x00,
0x00,0x11,0x11,0x13,0x1d,0x10,0x10,0x10,
0x0e,0x11,0x1f,0x11,0x11,0x0e,0x00,0x00,
0x1C,0x10,0x1B,0x12,0x1B,0x01,0x07,0x00,
0x04,0x0e,0x1f,0x04,0x1f,0x0e,0x04,0x00
};//自定义符号
code char *lcr[7]   = {"2000", " 200", "20.0","2.00", " 20", " 20","2.0"};
code char *feqc[3]  = {"100Hz"," 1KHz" ,"7.8KH"};
code char *zero[2]  = {"Open zero......","Short zero...OK."};
char binLian=0,ATA=0,ATB=0,ATD=0;
char OX[8]={0,0,0,0,0,0,8,0};
int DDS3=1, c3 = 0,c4 = 0; 
xdata char isQ=1,REL=0;
xdata uchar menu=1,menu2=0; //菜单变量
xdata float TR,TX; //清零数据
xdata float RELDAT; //相对测量数据
uint ph=0, phM=256, feq=977; //相位,phM相位步进值



void lcd_delay(int n){ //LCD专用延时函数
  //32MHz钟频下，约循环3000次延迟1毫秒
  int i,j;
  if(n<0)    { for(i=0;i< 30;i++); return; } //10us
  if(n== 0)  { for(i=0;i<150;i++); return; } //50us
  for(;n;n--){ for(j=0;j<3000;j++);        } //n毫秒
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

void lcd_putp(float a,float b,char bo,char n, float qmin){ //带单位显示复数,n是单位下限,qmin是最小位权值(用于限定有效数字)
  code uchar dwB[] = {'p','n',1,'m',0,'k','M','G'}; //单位表
  char i,j, c=0,digW=4, h=3, fh[2]={' ','+'};
  long d,q,qm,Q=1; //最高位权
  float f,g=1,k=1;
  if(bo==2){k=2;}
  if(a<0) fh[0] = '-', a = -a;
  if(b<0) fh[1] = '-', b = -b;
  if(a>b) f = a; else f = b;
  for(i=1;i<4;i++) Q *= 10;
  for(i=0;i<3;i++){ if(f*g >= k*1000) g/=1000, c++; } //以3位为单位移动小数点,右移
  for(i=0;i<n;i++){ if(f*g < k)     g*=1000, c--; }   //以3位为单位移动小数点,左移
  if ((n==1)&&(c==-1)){
  digW=2;                 //移动1位小数点
  }else if ((n==2)&&(c==-2)){
  if(feq!=7813) {digW=2;   //移动1位小数点
  }else{ digW=3; }         //移动2位小数点
  }else if ((n==4)&&(c==-4)){
  if(feq!=7813) {digW=2;   //移动1位小数点
  }else{ digW=3; }         //移动2位小数点
  }
  for(i=1;i<digW && f*g<k*Q;i++) g*=10,h--;           //继续移动小数点，使之满字
  qm = g*qmin;
  for(i=0;i<2;i++){
   if(i) d = b*g+0.5;      //取出实部
   else  d = a*g+0.5;      //取出虚部
   if(qm) d+=qm/2, d-=d%qm;//去除小于qmin的尾数
   if(d>=Q*20) d=(d+0.5)/10, h++;
   q = Q;
   if(d>=Q*10){
     d -= Q*10;
     if(fh[i]=='-') {lcd_puts("-1");
     }else          {lcd_puts(" 1");}
   }else{
   lcd_putc(fh[i]); if(bo==2)lcd_putc('0');      //显示符号
   }
   for(j=0; j<4; j++){  //数字输出
    lcd_putc(d/q+48);      //数字
    if(j==h) lcd_putc('.');//小数点
    d %= q, q /= 10;  
   }
   if(!bo||bo==2) break;     //不显示虚部
  }
  if((n!=1)&&(dwB[c+4]==0))lcd_putc(' '); else lcd_putc(dwB[c+4]);//单位
  if((n==1)&&(dwB[c+4]==0)) OX[6]=32; else OX[6]=8; //自动单位
}

void lcd_putf(float a, char n, float qmin) //带单位显示浮点数,n是单位下限
  { lcd_putp(a,0,0,n,qmin); }
void lcd_int(int a,char w){ //定宽显示正整数
  char i=0, s[5] = {' ',' ',' ',' ',' '};
  if(a<0) { a=-a; lcd_puts("-"); }
  else    lcd_puts(" ");
  do{
   s[i++] = a%10+48;
   a /= 10;
  }while(a);
  for(;w;w--) lcd_putc(s[w-1]);
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

void cs_RW(char rw){
 uchar i,*p = &cs;
 const int offs=512;
 if(rw){
  delay2(20); //等待(防止掉电误识别键盘而调用本函数)
  eraseEEP(offs);
  for(i=0;i<sizeof(cs);i++) writeEEP(i+offs,p[i]);
 }else{
  for(i=0;i<sizeof(cs);i++) p[i]=readEEP(i+offs);
 }
}

//==========================================================================
//==================================LCR主程序===============================
//==========================================================================
sfr P1M1=0x91; //P1端口设置寄存器
sfr P1M0=0x92; //P1端口设置寄存器
sfr P0M1=0x93; //P0端口设置寄存器
sfr P0M0=0x94; //P0端口设置寄存器
sfr P2M1=0x95; //P2端口设置寄存器
sfr P2M0=0x96; //P2端口设置寄存器
sfr P3M1=0xB1; //P3端口设置寄存器
sfr P3M0=0xB2; //P3端口设置寄存器


sbit spk=P2^3; //蜂鸣器
sbit Kb=P2^1;  //量程开关B
sbit Ka=P2^2;  //量程开关A
sbit DDS2=P1^2;//移相方波输出口
sbit K3=P1^7;
sbit K4=P1^6;
sbit K5=P1^5;  //7.8kHz滤波开关
sbit K6=P1^4;
sbit K8=P2^0;  //100Hz滤波开关
sbit K32=P1^1; //32kHz发生器


//==============低频信号DDS相关参数====================
//PCA相关寄存器
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


uchar *ph8 = (char*)&ph;    //ph的高8位地址
xdata float feqX=976.6; //实际输出频率
uchar code sinB[256]={
 //查询表中不可装载零值，否则会造成无中断产生
 255,255,255,255,255,255,254,254,253,252,252,251,250,249,248,247,246,245,243,242,240,239,237,236,234,232,230,229,227,225,222,220,
 218,216,214,211,209,206,204,201,199,196,194,191,188,185,183,180,177,174,171,168,165,162,159,156,153,150,147,144,140,137,134,131,
 128,125,122,119,116,112,109,106,103,100, 97, 94, 91, 88, 85, 82, 79, 76, 73, 71, 68, 65, 62, 60, 57, 55, 52, 50, 47, 45, 42, 40,
  38, 36, 34, 31, 29, 27, 26, 24, 22, 20, 19, 17, 16, 14, 13, 11, 10,  9,  8,  7,  6,  5,  4,  4,  3,  2,  2,  1,  1,  1,  1,  1,
   1,  1,  1,  1,  1,  1,  2,  2,  3,  4,  4,  5,  6,  7,  8,  9, 10, 11, 13, 14, 16, 17, 19, 20, 22, 24, 26, 27, 29, 31, 34, 36,
  38, 40, 42, 45, 47, 50, 52, 55, 57, 60, 62, 65, 68, 71, 73, 76, 79, 82, 85, 88, 91, 94, 97,100,103,106,109,112,116,119,122,125,
 128,131,134,137,140,144,147,150,153,156,159,162,165,168,171,174,177,180,183,185,188,191,194,196,199,201,204,206,209,211,214,216,
 218,220,222,225,227,229,230,232,234,236,237,239,240,242,243,245,246,247,248,249,250,251,252,252,253,254,254,255,255,255,255,255
};
uchar code fbB[256]={ //方波DDS查询表
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uchar chuX=0; //方波DDS初相
//==============低频信号DDS函数====================
void PWM_init(){ //把PCA置为PWM
  CMOD = 2;   //0000 0010 计数源选择,钟源取fosc/2
  CL = CH = 0;
  CCAP0L = CCAP0H = 192; //占空比为25%
  //CCPAM0=0x42;//0100 0010,PCA的模块0设置为PWM模式,无中断
  CCPAM0=0x53;//0101 0011,PCA的模块0设置为PWM模式,有中断，下降沿中断
  PPCA = 1;   //优先中断
  IPH |= 128;
  //CR = 1;   //开始计数
  EA = 1;     //开总中断
}

void PCAinter(void) interrupt 7 {//PCA中断
  uchar x,y;
  CCF0=0; //清除中断请求,以免反复中断
  x = *ph8;        //截断正弦相位累加器,取高8位
  y = x + chuX;    //方波相位
  CCAP0H = sinB[x];//正弦DDS输出
 if(DDS3) {  DDS2 = fbB[y];   //方波DDS输出
}else {DDS2 = 0;}
  ph += phM;       //相位累加
  K32 = ~K32;
}
void setDDS(uint f){ //参考时钟是c=(fosc/2)/256=32000000/2/256=62500,频率f=c*phM/2^16
 feq = f;
 phM=f*65536.0/62500; //phM=f*2^16/62500
 feqX = 62500.0*phM/65536*(1+cs.feq/10000.0); //实际输出频率
 ph = 0;              //高频时，使波形对称
 if(!f) CR=0; else CR=1;
}

//相位控制函数
xdata char xw=0; //相位
void set90(char k){ //设置方波的相位差
  k %= 4;
  if(k<0) k += 4;
  chuX = k*64;   //移相0~270度
  xw = k;
}
void set902() { set90(xw+1); } //相位步进

//==============量程控制函数====================
xdata char rng=1; //量程
void setRng(char k){//切换量程
 if(k>3) k=3;
 if(k<0) k=0;
 Ka = k & 2, Kb = k & 1;  //40欧--100k欧
 rng = k;
}
void setRng2(){ setRng( (rng+1)%4); } //量程步进

//==============增益控制函数====================
char curGain=1; //当前增益索引号
void setGain(char k){ //设置电路增益
  if(k>3) k=3;
  if(k<0) k=0;
  K4 = k & 2, K6 = k & 1; //1倍--27倍
  curGain = k;
}
void setGain2(){ setGain((curGain+1)%4); }


//==============AD非线性改正与过采样=================
uint getAD10(){ //250次采样,用reentrant申明，有的单片机无法运行
 uchar i;
 long c;
 for(i=0;i<250;i++) c += getAD2();
 return (c+12)/25;
}
uint getAD10b(){ //250次采样,用reentrant申明，有的单片机无法运行
 xdata uint c = 0, c2 = 0;
 uchar i;
 for(i=0;i<250;i++){
  ADC_CONTR |= 0x08;             //00001000,置ADC_START=1启动A/D 转换
  while ( !(ADC_CONTR & 0x10) ); //等待A/D转换结束(ADC_FLAG==0)
  ADC_CONTR &= 0xE7;             //11100111,置ADC_FLAG=0清除结束标记, 置ADC_START=0关闭A/D 转换
  c  += ADC_resl;
  c2 += ADC_res;
 }
 c3 = c;
 c = ( c2*4L + c )/25;
 c3= ( c2*4L + c3 ) -c*25;
 return c;
}



//==============LCR测量====================
code float ga[4] =  {  1,   3,   9,  27 }; //增益表
code float dwR[4] = { 40, 1e3, 1e4, 1e5 }; //各档电阻表
xdata int Vxy[12]={0,0,0,0,1,1}; //Vxy[Vx1,Vy1,Vx2,Vy2,g1,g2]
xdata char Sxw[4]={0,1,0,1};    //保存正确相位
xdata int Vz[24];               //LCR测量结果队
xdata uchar tim=0,tims=0;
xdata char pau=0; //暂停坐标自动旋转


#define Vfull 9600
#define gad (9600/30)
uchar mT = 6; //测量速度,mT取值为6或12或24时，可以消除数字噪声，尾数不动，但不利于于取平均
//==============设置频率====================
xdata char feqK=1; //频率索引号
void setF(char k){
  if(k==-1) k = (feqK+1)%3; //步进
  feqK = k;
  if(k==0) { setDDS(100);   K5=0; K8=1; mT=12; } //置为100Hz
  if(k==1) { setDDS(977);   K5=0; K8=0; mT=6;  } //置为1kHz
  if(k==2) { setDDS(7813);  K5=1; K8=0; mT=6;  } //置为7.8125kHz
  TH1 = 150, TL1 = 171; //置为20ms
  tims = 0;
  tim = 0;
  ph = 0;
}
int absMax(int a,int b){ //取两个数绝对值最大者
  if(a<0) a = -a;
  if(b<0) b = -b;
  if(b>a) a = b;
  return a;
}

char yc1=0,yc2=0; //溢出标识
char slw = 1; //降速倍率
char chg=0;   //量程切换标记
void timerInter1(void) interrupt 3 {//T1中断,LCR数据采集
 code int y[5]={0,-15,30,24,0}; //满度用30
 code int x[5]={4900,5090,5140,5500,6120};
  char g,gb,Rb,i; int c=0,cc=0;
  tims++;
  if(tims>=mT/slw) tims = 0, tim++, c = 1;
  if(tim>=4) tim=0;
  if(pau) return;
  if(!c) return; //tim未进位触发
  c = getAD10b();   //读取电压值
  c4= c;
  cc= c3;
  c -= cs.zo[feqK];
if(cs.ak!=0){
 for(i=0;i<4;i++){ //非线性改正
  if(c<x[i]||c>=x[i+1]) continue;
  c += cs.ak*( y[i] + (y[i+1]-y[i]) * ((int)(c-x[i])) / (x[i+1]-x[i]) ) / 30;
  break;
 } 
}
  
  Vxy[tim] = xw<2 ? c : -c;  //保存当前电压
  Vxy[tim+6] = xw<2 ? cc : -cc;  //保存当前电压
  Vxy[tim/2+4] = curGain;    //保存当前增益
  Sxw[tim] = ( Sxw[tim]+(c<0 ? 2 : 0) )%4;  //相位翻转(预测下次的相位采用值)
  if(tim==1||tim==3){ //上下臂切换
    if(tim==1) { K3=1, c = absMax(Vxy[2],Vxy[3]), g=Vxy[5]; yc2 = c>Vfull ? 1:0; }//切换到下臂
    if(tim==3) { K3=0, c = absMax(Vxy[0],Vxy[1]), g=Vxy[4]; yc1 = c>Vfull ? 1:0; }//切换到上臂
    gb=g, Rb=rng;
    if(c>Vfull){ if(g==0&&rng>0&&K3&&isQ) Rb=rng-1; gb=0; }
    else if(c<gad*1 ) gb = g+3; //增加27倍
    else if(c<gad*3 ){ gb = g+2;if(c<gad*2&&rng>=1&&rng<=2&&K3&&isQ ) { gb=0; Rb=rng+1; }} //增加9倍
    else if(c<gad*9)  gb = g+1; //增加3倍
	if(gb>3) gb = 3;
    if(g==3&&rng<3&&K3&&isQ) { gb=0; Rb=rng+1; }
    if(g==0&&c<gad*1&&rng<3&&K3&&isQ) { gb=0; Rb=rng+1; }

	setRng(Rb); setGain(gb); //置量程
    if(gb!=g || Rb!=rng) slw = 2, chg++; //量程正在改变，则加速测量
    if(tim==3){ if(!chg) slw=1; chg=0; }
  }
  set90( Sxw[ (tim+1)%4 ] ); //相位旋转
  if(tim==3){ for(i=0;i<12;i++) Vz[i+12]=Vz[i], Vz[i]=Vxy[i]; }
}


void showR(char binLian, char showk){ //显示LCR
  xdata float a=0,b=0,c=0,qq=0,dd=0,e,w,L,C;
  xdata int gr=cs.R[rng], g1=cs.g1, g2=cs.g2;
  xdata int g12 = g1 + g2;          //增益最大补偿
  xdata int j12 = (int)cs.j1+cs.j2; //相位最大补偿
  xdata float RELOUT;
  xdata long RELTMP;
  xdata float JD = 0, G = 0, cJD;   //补偿变量
  xdata float v[4];
  xdata char ATC=0,ATE=0,d2=0,d3=1;
  xdata float LCR[3];
  //LCR计算
  if(feqK<0||feqK>2) return;
  if(feq==7813&&rng==3) gr += cs.R4b; //7.8kHz时下臂修正量
  //可控增益单元的增益修正、相位补偿量
  if(feq==7813) g2 += cs.G2b;  //7.8kHz时9倍档修正量
  if(Vz[4] == 1) JD += cs.j1,  G += g1;
  if(Vz[4] == 2) JD += cs.j2,  G += g2;
  if(Vz[4] == 3) JD += j12,    G += g12;
  if(Vz[5] == 1) JD -= cs.j1,  G -= g1;
  if(Vz[5] == 2) JD -= cs.j2,  G -= g2;
  if(Vz[5] == 3) JD -= j12,    G -= g12;
  JD = (JD - cs.J[rng]) * feqX/7813/2000;
  if(feq==977) JD -= cs.phx/2000.0;
  cJD = 1 - JD*JD/2;
  v[0] = Vz[0]+Vz[12]+(Vz[6]+Vz[18])/25.0;
  v[1] = Vz[1]+Vz[13]+(Vz[7]+Vz[19])/25.0;
  v[2] = Vz[2]+Vz[14]+(Vz[8]+Vz[20])/25.0;
  v[3] = Vz[3]+Vz[15]+(Vz[9]+Vz[21])/25.0;
  a = (+( 1.0*v[2]*v[2] + 1.0*v[3]*v[3] )*((ga[Vz[4]] / ga[Vz[5]]) /(dwR[rng]*(1+gr/10000.0))))+(+( 1.0*v[2]*v[2] + 1.0*v[3]*v[3] )*((ga[Vz[4]] / ga[Vz[5]]) /(dwR[rng]*(1+gr/10000.0))))*G/10000;  //增益补偿
  b = (-( 1.0*v[0]*v[2] + 1.0*v[1]*v[3] ))*cJD - (-( 1.0*v[2]*v[1] - 1.0*v[0]*v[3] ))*JD; //相位补偿
  c = (-( 1.0*v[0]*v[2] + 1.0*v[1]*v[3] ))*JD + (-( 1.0*v[2]*v[1] - 1.0*v[0]*v[3] ))*cJD; //相位补偿  
 if(showk){
  w = 2*3.1415926*feqX;
  if((showk==5)||(showk==8)||(showk==10)||(showk==13)||(showk==15)||(showk==18)||(showk==19)){d3=0;}

  if((showk==1)||(showk==2)||(showk==3)){
  showk=0;
  DDS2 = 0;
  lcd_puts("Z=");
  lcd_putf(c4-cs.zo[feqK],0,0);
  return;
}
  if(binLian){ //并联
    e = (b*b+c*c)/a;
  if(d3){
    lcd_puts("R=");
	lcd_putp(e/b, 0,2,1, 1e-4); //显示并联复阻抗,显示到毫欧
  }else{
  if((showk==10)||(showk==13)||(showk==15)||(showk==18)){
	C = -c/e/w;
	L = +e/c/w;
    if(C>-2e-12) { lcd_putp(C, 0,2,4, 1e-14); lcd_putc('F'); } //显示并联C值,显示到pF
    else         { lcd_putp(L, 0,2,2, 5e-9 ); lcd_putc('H'); } //显示并联L值,显示到uH
  }else{
    lcd_putc(10);lcd_puts("=");
    lcd_putf(atan2(c,b)*180/3.1415926,0,0); //显示角度
    lcd_puts("    ");
 }

}
  }else{ //串联
    e = a;
  if(d3){
    lcd_puts("R=");
	lcd_putp(b/e, c/e, 2, 1, 1e-5); lcd_putc(OX[6]); //定义欧姆符号_显示串联复阻抗,显示到毫欧
  }else{
  if((showk==10)||(showk==13)||(showk==15)||(showk==18)){
	C = -e/c/w;
	L = +c/e/w;
    if(L<-2e-8) { lcd_putp(C, 0,2,4, 1e-14); lcd_putc('F'); } //显示C值,显示到pF
    else        { lcd_putp(L, 0,2,2, 5e-9 ); lcd_putc('H'); } //显示L值,显示到uH
  }else{
    lcd_putc(10);lcd_puts("=");
    lcd_putf(atan2(c,b)*180/3.1415926,0,0); //显示角度
    lcd_puts("    ");
  }

  }
 }
 return;
}

  if(binLian==20){ //测量开路残余值
  if(rng==3){
  a = (b*b+c*c)/a;
  TR += b/a, TX += c/a; //开路残余导抗
  }
  if(rng==0){
  TR += b/a, TX += c/a; //短路残余阻抗
  }
  return;
  }

  if(binLian==21){ //是否应用清零
    isQ = (isQ+1)%2;
    return;
  }

  w = 2*3.1415926*feqX;
  if(isQ){ //自动手动
  if(rng==0){ 
    b -= cs.QRs[feqK]*a, c -= cs.QXs[feqK]*a;//短路清零
  }
  if(rng>1){
    a = (b*b+c*c)/a;	
    b -= cs.QRo[feqK]*a, c -= cs.QXo[feqK]*a; //开路清零
    a = (b*b+c*c)/a;
 }

  if(ATA!=0){
  if(ATD==0) { if(rng>1)ATE=1;else ATE=0;  } //置为100Hz
  if(ATD==1) { ATE=1;  } //置为1kHz
  if(ATD==2) { ATE=0;  } //置为7.8125kHz
  }else      { if(rng>1)ATE=1;else ATE=0; } 

  if(feq==100)  LCR[0]=2e3,LCR[1]=2e-1,LCR[2]=2e7,d2=1;
  if(feq==977)  LCR[0]=2e2,LCR[1]=2e-2,LCR[2]=2e7,d2=2;
  if(feq==7813) LCR[0]=2e1,LCR[1]=2e-3,LCR[2]=2e6,d2=3;

  if(ATE){ //并联
    e = (b*b+c*c)/a;
	C = -c/e/w;
	L = +e/c/w;
    if(C>-2e-12) { ATC=1; } 
    else         { ATC=2; } 
    if(fabs(e/b)<fabs(e/c)) { ATC=3; } 
  }else{ //串联
    e = a;
	C = -e/c/w;
	L = +c/e/w;
    if(L<-2e-8)  { ATC=1; } 
    else         { ATC=2; } 
    if(fabs(b/e)>fabs(c/e)) { ATC=3; } 
   }


  qq = fabs(c/b); //计算Q
  if(!b||qq>999) qq = 999;
  dd = fabs(b/c); //计算D
  if(!c||dd>=10) dd = 9.999;

  if(ATA!=0){ATC=ATA;}


  lcd_goto1(0);
  if(ATC==0){

  }else if(ATC==1){
  if(ATE){ //并联
  lcd_puts("Cp"); 
    if(C>-2e-12) { 
    if(fabs(C)<LCR[1]) {lcd_putp(C, 0,2,4, 1e-14); lcd_putc('F');  //显示并联C值,显示到pF
    }else{lcd_puts(" C>");lcd_puts(lcr[d2]);lcd_puts("mF");}
    } 
    else         { lcd_puts(" ---OL---"); } 
  }else{ //串联
  lcd_puts("Cs"); 
    if(L<-2e-8)  {
    if(fabs(C)<LCR[1]) {lcd_putp(C, 0,2,4, 1e-14); lcd_putc('F');  //显示串联C值,显示到pF
    }else{lcd_puts(" C>");lcd_puts(lcr[d2]);lcd_puts("mF");}
    } 
    else         { lcd_puts(" ---OL---"); } 
  }

  }else if(ATC==2){
  if(ATE){ //并联
  lcd_puts("Lp"); 
    if(C>-2e-12) { lcd_puts(" ---OL---"); } 
    else         {
    if(fabs(L)<LCR[0]) { lcd_putp(L, 0,2,2, 5e-9 ); lcd_putc('H');   //显示并联L值,显示到uH
    }else{lcd_puts(" L>");lcd_puts(lcr[d2-1]);lcd_puts(" H");}
    } 
  }else{ //串联
  lcd_puts("Ls"); 
    if(L<-2e-8)  { lcd_puts(" ---OL---"); } 
    else         {
    if(fabs(L)<LCR[0]) { lcd_putp(L, 0,2,2, 5e-9 ); lcd_putc('H');   //显示串联L值,显示到uH
    }else{lcd_puts(" L>");lcd_puts(lcr[d2-1]);lcd_puts(" H");}
    } 
  }


  }else if(ATC==3){
  if(ATE){ //并联
  lcd_puts("Rp"); 
   if(fabs(e/b)<LCR[2]) {  lcd_putp(e/b, 0,2,1, 1e-4); lcd_putc(OX[6]);} //显示电阻
   else                 { lcd_puts(" R>");lcd_puts(lcr[d2+3]);lcd_puts(" M");lcd_putc(OX[6]); } 
  }else{ //串联
  lcd_puts("Rs"); 
   if(fabs(b/e)<LCR[2]) { lcd_putp(b/e, 0,2,1, 1e-4); lcd_putc(OX[6]);} //显示电阻
   else                 { lcd_puts(" R>");lcd_puts(lcr[d2+3]);lcd_puts(" M");lcd_putc(OX[6]); } 

  }
 }

  lcd_goto2(0);
  if(REL) {
  lcd_putc(4);lcd_puts("=");
  if(ATC==0){
  }else if(ATC==1){if(RELDAT==0){RELDAT=C;RELOUT=0;}else{RELOUT=(C-RELDAT)/(RELDAT*0.01);}
  }else if(ATC==2){if(RELDAT==0){RELDAT=L;RELOUT=0;}else{RELOUT=(L-RELDAT)/(RELDAT*0.01);}
  }else if(ATC==3){
  if(ATE){ //并联
  if(RELDAT==0){RELDAT=e/b;RELOUT=0;}else{RELOUT=((e/b)-RELDAT)/(RELDAT*0.01);}
  }else{ //串联
  if(RELDAT==0){RELDAT=b/e;RELOUT=0;}else{RELOUT=((b/e)-RELDAT)/(RELDAT*0.01);}
  }
  }
   if(RELOUT<-99.9)RELOUT=-100;
   if(RELOUT>99.9)RELOUT=100;

  if (fabs(RELOUT)==100){
  if (RELOUT>99.9) lcd_puts("   OL  % "); else lcd_puts("-  OL  % ");
  }else{
   RELTMP=RELOUT*1000+0.5;
   lcd_putf(RELTMP/1000.0,0,0); lcd_puts("% ");
  }
  if(!ATA){ATA=ATC;}

}else{
  RELDAT=0;
        if(ATB==0){
  lcd_puts("Q="); 
  lcd_putf(qq,0,0);lcd_puts("  "); //显示Q
  }else if(ATB==1){
  lcd_puts("D="); 
  lcd_putf(dd,0,0); lcd_puts("  ");//显示D
  }else if(ATB==2){
  lcd_putc(10);lcd_putc('=');
  lcd_putf(atan2(c,b)*180/3.1415926,0,0);lcd_puts("  "); //显示角度
  }else if(ATB==3){
  if(ATC==3){
  if(ATE){ //并联
  lcd_puts("Xp");
   if(fabs(e/c)<LCR[2]) { lcd_putf(e/c, 1, 1e-4); lcd_putc(OX[6]);} //显示电抗
   else                 { lcd_puts(" X>");lcd_puts(lcr[d2+3]);lcd_puts("M");lcd_putc(OX[6]); } 
  }else{ //串联
  lcd_puts("Xs");
   if(fabs(c/e)<LCR[2]) { lcd_putp(c/e, b/e, 0, 1, 1e-4); lcd_putc(OX[6]);} //显示电抗
   else                 { lcd_puts(" X>");lcd_puts(lcr[d2+3]);lcd_puts("M");lcd_putc(OX[6]); } 

  }
  }else{
  if(ATE){ //并联
  lcd_puts("Rp");
   if(fabs(e/b)<LCR[2]) {lcd_putf(e/b, 1, 1e-4); lcd_putc(OX[6]);} //显示电阻
   else                 { lcd_puts(" R>");lcd_puts(lcr[d2+3]);lcd_puts("M");lcd_putc(OX[6]); } 
  }else{ //串联
  lcd_putc(11);lcd_putc('R');
   if(fabs(b/e)<LCR[2]) { lcd_putp(b/e, c/e, 0, 1, 1e-4); lcd_putc(OX[6]);} //显示电阻
   else                 { lcd_puts(" R>");lcd_puts(lcr[d2+3]);lcd_puts("M");lcd_putc(OX[6]); } 
  }
  }
 }
}


  lcd_goto1(11);
  if(ATA==0||ATD==0){
  if(ATA==0) lcd_puts(" AUTO");
  if(ATA==1) lcd_puts(" AU-C");
  if(ATA==2) lcd_puts(" AU-L");
  if(ATA==3) lcd_puts(" AU-R");
  }else{
  if(ATE) lcd_puts(" P "); else lcd_puts(" S ");

  if(feq==100)  lcd_putc('A');
  if(feq==977)  lcd_putc('B');
  if(feq==7813) lcd_putc('C');
  lcd_putc(rng+49); //显示量程
}
  lcd_goto2(11);
  lcd_puts(feqc[feqK]);
  }else{
  //显示量程信息
  lcd_goto1(0);
  if(isQ)     lcd_putc('Z');  else lcd_putc('H'); //显示自动手动
  if(binLian) lcd_putc('p');  else lcd_putc('s'); //显示串联并联

  lcd_goto2(0);     //显示频率
  if(feq==100)  lcd_putc('A');
  if(feq==977)  lcd_putc('B');
  if(feq==7813) lcd_putc('C');
  lcd_putc(rng+49); //显示量程
  if(yc1&&rng!=3)     { lcd_goto1(2); lcd_puts(" Overflow,high");lcd_goto2(2); lcd_puts("         ");} //未知高阻溢出
  if(yc2&&rng!=0)     { lcd_goto1(2); lcd_puts(" Overflow,low ");lcd_goto2(2); lcd_puts("         ");} //未知低阻溢出
  if(yc1||yc2){return; } //第2行清空

  //电学量显示  if(!a) { lcd_cls(); lcd_puts("DIV 0"); return; }

  if(binLian){ //并联
    e = (b*b+c*c)/a;
    lcd_goto1(2);
	lcd_putf(e/b, 1, 1e-4); //显示并联复阻抗,显示到毫欧
	lcd_putf(e/c, 1, 1e-4); //显示并联复阻抗,显示到毫欧
    lcd_goto2(2);
	C = -c/e/w;
	L = +e/c/w;
    if(C>-2e-12) { lcd_putf(C, 4, 1e-14); lcd_putc('F'); } //显示并联C值,显示到pF
    else         { lcd_putf(L, 2, 5e-9 ); lcd_putc('H'); } //显示并联L值,显示到uH
  }else{ //串联
    e = a;
    lcd_goto1(2);
	lcd_putp(b/e, c/e, 1, 1, 1e-4); lcd_putc(OX[6]); //定义欧姆符号_显示串联复阻抗,显示到毫欧
    lcd_goto2(2);
	C = -e/c/w;
	L = +c/e/w;
    if(L<-2e-8) { lcd_putf(C, 4, 1e-14); lcd_putc('F'); } //显示C值,显示到pF
    else        { lcd_putf(L, 2, 5e-9 ); lcd_putc('H'); } //显示L值,显示到uH
  }


  c = fabs(c/b); //计算Q
if(!b||c>999) c = 999;
  lcd_putf(c,0,0); //显示Q
}
}
//void timerInter(void) interrupt 1 {}//T0中断
main(){
 uchar i=0,s=0,key=0;
 uchar dispN=0; //显示扫描索引
 uchar spkN=0;  //蜂鸣器发声时长
 uint nn=0;
 int kn=0;
 char mo=0; //编辑显示开关
 lcd_init(); //初始化LCD
 lcd_cur0(); //隐藏光标
 lcd_B(0, 0x40, 0); //定义欧姆符号_设置CGRAM地址,写指令
 for(i=0;i<40;i++)lcd_B(1, 0x00+path[i], 0); //定义欧姆符号_把数据送入CGRAM地址内
 lcd_goto1(0);//定义欧姆符号_光标定位
 lcd_puts("LCR 3.0");
 lcd_goto2(0);
 lcd_puts("XJW Putian,2012");
 delay2(500); //启动延时
 eepEn= 12345;
 cs_RW(0);   //读EEPROM

 TCON=0, TMOD=0x12; //将T0置为自动重装定时器，T1置为定时器
 TH1 = 0, TL1 = 0;
 TR1=1;  //T1开始计数
 TR0=0;  //T0暂停计数
 ET1=1;  //T1开中断
 ET0=1;  //T1开中断
 EA=1;   //开总中断
 //PT0=1;  //设置优先级

 set_channel(0); //设置AD转换通道
 P2M0 = 0xFF;    //P2.01234567置为推勉输出
 P1M0 = 0xFE;    //P1.1234567置为推换口
 P1M1 = 0x01;    //P1.0置为高阻抗
 P2 = 0x0F;


 PWM_init(); //DDS初始化
 set90(2);   //初始设置相位
 setRng(1);  //初始设置量程
 setGain(1); //初始设置增益
 setF(1);    //DDS初始设置为1kHz


 while(1){
  //显示disp
  nn++;
  //扫描键盘
  key = ~P3;
  if(key&&kn<255) kn++; else kn=0;
  for(i=0;key;i++) key/=2; key=i;
  if(menu==7&&key<3){
  if(kn==4) spkN=10,kn=-14; else key=0;   //校准数值加速
  }else{
  if(kn==4) spkN=10; else key=0;   //当按下一定时间后key才有效。spkN发声时长设置
  }

  //菜单系统
  if(key==8){//菜单键
    lcd_cls();    lcd_puts(" LCR Set Options");
    lcd_goto2(0); lcd_puts(" Exit:X  Ver:5.5");
    menu=0; key=0; mo=0; pau=0;
    for(i=0;i<3;i++)OX[i]=3.1415;

    delay(10000);
  }
  if(menu>=1 && menu<=3){
    if(key==7) setRng2(); //量程步进
    if(key==6) setF(-1);  //设置频率
    delay(10000);
  }
  if(menu==0){ //显示量程和菜单
    if(key) lcd_cls();
	if(key>=1 && key<=7) {menu = key, menu2 = 0;if (key==1)isQ=1;}
	key = 0; 
    DDS3 = 1;
    delay(10000);
  }
  if(menu==1){ //LCR测量
    if(key==1) if(isQ){ ATA++;ATD=0;REL=0;if(ATA>3){ATA=0;}}else{binLian = (binLian+1)%2;} //L／C／R 模式
    if(key==2) if(isQ){ ATB++;REL=0;if(ATB>3){ATB=0;}} //副参切换
    if(key==3) if(!isQ||ATA==0){showR(21,0);REL=0;}else{ ATD++;REL=0;if(ATD>2){ATD=0;}} //自动手动切换——串并联测量模式选择
    if(key==4) {
  if((rng==3)||(rng==0)){
  lcd_cls();if(rng==3){lcd_puts(zero[0]);}else{lcd_puts(zero[1]);}delay2(100);
  for(i=0;i<3;i++){
  lcd_goto2(2);
  if(feq==100)  delay2(150);
  lcd_puts(feqc[feqK]);lcd_goto2(9);lcd_puts(" ");
  delay2(100);
  TR = 0, TX = 0; 
  for(s=0;s<4;s++){lcd_goto2(9);lcd_putc(51-s);if(feq==100){delay2(150);}else{delay2(75);}showR(20,0);}
  if(rng==3){
  cs.QRo[feqK] = TR/4.0, cs.QXo[feqK] = TX/4.0; //开路残余导抗
  }
  if(rng==0){
  cs.QRs[feqK] = TR/4.0, cs.QXs[feqK] = TX/4.0; //短路残余阻抗
  }
  setF(-1);
  }
  cs_RW(1);
  lcd_goto1(0);if(rng==3){lcd_puts(zero[0]);}else{lcd_puts(zero[1]);}delay2(100);
  }
  REL=0;
  } //清零
    OX[7]++;
    if(OX[7]>15){OX[7]=0;showR(binLian,0);}
    delay(12000);
  }
    if(key==5) {REL = (REL+1)%2;spkN=0;}

  if(menu==2){
    lcd_goto1(0);
    lcd_puts("up:");  lcd_putc(Vxy[4]+48);
    lcd_puts(" dw:"); lcd_putc(Vxy[5]+48);
   delay(10000);
  }
  if(menu==3){ //手动调试
    pau = 1;
    if(key==1) setGain2();//增益控制
    if(key==2) { };
    if(key==3) K3=~K3;    //切换上下臂
    if(key==4) set902();  //相位旋转
    lcd_goto1(0);
    lcd_puts(" xw="); lcd_putc(xw+48);      //相位索引号
    lcd_puts(" K3="); lcd_putc(K3?49:48);   //K3状态
    lcd_puts(" Ga="); lcd_putc(curGain+48); //增益索引号
    lcd_goto2(0);
    if(nn%32==0) lcd_int(getAD10(),5);
    delay(10000);
  }
  if(menu>=4 && menu<=5){
    delay(10000);
  }
  if(menu==6){ //频率修正
    if(key==1) cs.feq += 1; //X键增
    if(key==2) cs.feq -= 1; //R键减
    if(key==3) { cs_RW(1); setF(feqK); } //L键保存
    if(OX[0]!=cs.feq){ lcd_cls();OX[0]=cs.feq;
    lcd_goto1(0); lcd_puts("Feq correct");
    lcd_goto2(0); lcd_putf(cs.feq,0,0);}
	delay(10000);
  }
  if(menu==7){ //校准
 	code char *csR[19]   = {
     "Z0 :000", "Z1 :000", "Z2 :000",
     "R1 : 40", "R1X: 40",
     "ak : 25",
     "R2 : 1k", "R2X: 1k",
     "R3 :10k", "R3X:10k",
     "R4 :.1M", "R4b:.1M", "R4X:.1M",
     "G1 : 3k", "G1X: 3k",
     "G2 : 9k", "G2b: 9k", "G2X: 9k",     
     "phX: 1k"
     };
    char *p,bc=1, feqD=1,rngD=1;
	static char kc=0;
	isQ = 0;
    lcd_goto2(5);
    if(menu2==0)  p = cs.zo+0,      feqD=0, rngD=0; //100Hz零点校准,接入20k欧电阻
    if(menu2==1)  p = cs.zo+1,      feqD=1, rngD=0; //1kHz零点校准,接入20k欧电阻
    if(menu2==2)  p = cs.zo+2,      feqD=2, rngD=0; //7.8kHz零点校准,接入20k欧电阻
    if(menu2==3)  p = cs.R+0,       feqD=1, rngD=0; //VI变换器模值校准,接入40欧
    if(menu2==4)  p = cs.J+0,       feqD=2, rngD=0; //40R相位
    if(menu2==5)  p = &cs.ak,       feqD=1, rngD=0; //AD校准
    if(menu2==6)  p = cs.R+1,       feqD=1, rngD=1; //VI变换器模值校准,接入1k欧
    if(menu2==7)  p = cs.J+1,       feqD=2, rngD=1; //1k相位
    if(menu2==8)  p = cs.R+2,       feqD=1, rngD=2; //VI变换器模值校准,接入10k欧
    if(menu2==9)  p = cs.J+2,       feqD=2, rngD=2; //10k相位
    if(menu2==10) p = cs.R+3,       feqD=1, rngD=3; //VI变换器模值校准,接入100k欧
    if(menu2==11) p = &cs.R4b,      feqD=2, rngD=3; //7.8kHz频率100k档幅度校准
    if(menu2==12) p = cs.J+3,       feqD=2, rngD=3; //100k相位
    if(menu2==13) p = &cs.g1,       feqD=1, rngD=1; //运放3倍增益校准,接入3.3k欧电阻
    if(menu2==14) p = &cs.j1,       feqD=2, rngD=1; //3倍档相位
    if(menu2==15) p = &cs.g2,       feqD=1, rngD=1; //运放9倍增益校准,接入10k欧电阻
    if(menu2==16) p = &cs.G2b,      feqD=2, rngD=1; //7.8kHz频率9倍档幅度校准
    if(menu2==17) p = &cs.j2,       feqD=2, rngD=1; //9倍档相位
    if(menu2==18) p = &cs.phx,      feqD=1, rngD=1; //1kHz以下相位校准,接入1k欧电阻
    if(menu2<3) DDS3=0; else DDS3=1;
    if(key==1) *p += bc; //X键增
    if(key==2) *p -= bc; //R键减
    if(key==3) { cs_RW(1); setF(feqK); } //L键保存
    if(key==4) *p = 0;   //C键清除
	if(key==5) { if(menu2==0) menu2=18; else menu2--; mo=0; }
	if(key==6) { if(menu2==18)menu2=0;  else menu2++; mo=0; }
	if(key==7) mo = (mo+1)%2;
	if(key==4){ //恢复到默认值
	  if(++kc==5){
	   kc = 0;
       cs.j1 = 36, cs.j2 = 34;
       cs.g1 = 0,  cs.g2 = 0;
       cs.zo[0] = 76;
       cs.zo[1] = 76;
       cs.zo[2] = 70;
       cs.J[0] = cs.J[1] = 0, cs.J[2] = 4, cs.J[3] = 48;
       cs.R[0] = cs.R[1] = cs.R[2] = cs.R[3] = 0;
       cs.R4b = cs.G2b = cs.phx = cs.feq = 0;
	   cs.ak = 0;
	   for(i=0;i<3;i++) cs.QRs[i] = cs.QXs[i] = cs.QRo[i] = cs.QXo[i] = 0;
	  }
	}
	else { if(key) kc=0; }
	//显示
	if(mo){
     OX[1]=3.1415;OX[2]=3.1415;
	 if(feqD!=feqK) setF(feqD);
	 if(rngD!=rng)  setRng(rngD);
     if(menu2>=3&&menu2<=7) showR(0,0);
     else showR(1,0);
	}else{
	 if(feqD!=feqK) setF(feqD);
	 if(rngD!=rng)  setRng(rngD);
     if(menu2>=3&&menu2<=7) showR(0,menu2+1);
     else showR(1,menu2+1);
    if(OX[1]!=*p||OX[2]!=menu2){ OX[1]=*p;OX[2]=menu2;
	  lcd_cls();
      lcd_goto1(0); lcd_puts(csR[menu2]);lcd_puts(" X:+ R:-"); //输出参数名称
      lcd_goto2(0); lcd_int(*p,3);     //输出参数值
      }
     delay(10000);
    }
  }
if (menu!=7){
  if(spkN) spkN--, spk=0; else spk=1; //键盘发声
}

 }//while end
}
//==========================================================================

