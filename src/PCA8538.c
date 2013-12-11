
// STC12C5A60S2 PCA8538驱动

/*
MOSI、MOSI和SCK要接4.7K上拉电阻才有显示。

NXP PCA8538 COG SPI接口连线

序号  COG            单片机

2     SCL---------->SCK     
4     SDAOUT------->MISO
5     SDAIN-------->MOSI
10    VDD---------->VCC
12    VSS---------->GND
15    GND---------->GND
16    5V----------->VCC

跳冒JP1——JP5接VSS，JP6短接，JP7不接
*/

#include "config_cog.h"

xdata char curr_x=0,curr_y=0;

/* 变量定义 ---------------------------------------------------------*/ 
const uchar code AscLib[]= 
{
	0x00,0x00,0x00,0x00,0x00, // - -
	0x00,0x00,0xFA,0x00,0x00, // -!-
	0x00,0xE0,0x00,0xE0,0x00, // -"-
	0x28,0xFE,0x28,0xFE,0x28, // -#-
	0x24,0x74,0xDE,0x54,0x48, // -$-
	0xC4,0xC8,0x10,0x26,0x46, // -%-
	0x6C,0x92,0x6A,0x04,0x0A, // -&-
	0x00,0x20,0xC0,0x80,0x00, // -'-
	0x00,0x38,0x44,0x82,0x00, // -(-
	0x00,0x82,0x44,0x38,0x00, // -)-
	0x44,0x28,0xFE,0x28,0x44, // -*-
	0x10,0x10,0xFE,0x10,0x10, // -+-
	0x02,0x0C,0x08,0x00,0x00, // -,-
	0x10,0x10,0x10,0x10,0x10, // ---
	0x00,0x06,0x06,0x00,0x00, // -.-
	0x04,0x08,0x10,0x20,0x40, // -/-
	0x7C,0x8A,0x92,0xA2,0x7C, // -0-
	0x00,0x42,0xFE,0x02,0x00, // -1-
	0x46,0x8A,0x92,0x92,0x62, // -2-
	0x84,0x82,0x92,0xB2,0xCC, // -3-
	0x18,0x28,0x48,0xFE,0x08, // -4-
	0xE4,0xA2,0xA2,0xA2,0x9C, // -5-
	0x3C,0x52,0x92,0x92,0x8C, // -6-
	0x80,0x8E,0x90,0xA0,0xC0, // -7-
	0x6C,0x92,0x92,0x92,0x6C, // -8-
	0x62,0x92,0x92,0x94,0x78, // -9-
	0x00,0x6C,0x6C,0x00,0x00, // -:-
	0x02,0x6C,0x6C,0x00,0x00, // -;-
	0x10,0x28,0x44,0x82,0x00, // -<-
	0x28,0x28,0x28,0x28,0x28, // -=-
	0x00,0x82,0x44,0x28,0x10, // ->-
	0x40,0x80,0x9A,0xA0,0x40, // -?-
	//0x7C,0x82,0xBA,0xAA,0x7A, // -@-      	
0x2E, 0x31, 0x01, 0x31, 0x2E,	//欧姆
	0x3E,0x48,0x88,0x48,0x3E, // -A-
	0xFE,0x92,0x92,0x92,0x6C, // -B-
	0x7C,0x82,0x82,0x82,0x44, // -C-
	0xFE,0x82,0x82,0x82,0x7C, // -D-
	0xFE,0x92,0x92,0x92,0x82, // -E-
	0xFE,0x90,0x90,0x90,0x80, // -F-
	0x7C,0x82,0x8A,0x8A,0x4E, // -G-
	0xFE,0x10,0x10,0x10,0xFE, // -H-
	0x00,0x82,0xFE,0x82,0x00, // -I-
	0x04,0x02,0x82,0xFC,0x80, // -J-
	0xFE,0x10,0x28,0x44,0x82, // -K-
	0xFE,0x02,0x02,0x02,0x02, // -L-
	0xFE,0x40,0x30,0x40,0xFE, // -M-
	0xFE,0x20,0x10,0x08,0xFE, // -N-
	0x7C,0x82,0x82,0x82,0x7C, // -O-
	0xFE,0x90,0x90,0x90,0x60, // -P-
	0x7C,0x82,0x8A,0x84,0x7A, // -Q-
	0xFE,0x90,0x98,0x94,0x62, // -R-
	0x64,0x92,0x92,0x92,0x4C, // -S-
	0x80,0x80,0xFE,0x80,0x80, // -T-
	0xFC,0x02,0x02,0x02,0xFC, // -U-
	0xF8,0x04,0x02,0x04,0xF8, // -V-
	0xFE,0x04,0x18,0x04,0xFE, // -W-
	0xC6,0x28,0x10,0x28,0xC6, // -X-
	0xC0,0x20,0x1E,0x20,0xC0, // -Y-
	0x86,0x8A,0x92,0xA2,0xC2, // -Z-
	0xFE,0xFE,0x82,0x82,0x00, // -[-
	0x40,0x20,0x10,0x08,0x04, // -\-
	0x00,0x82,0x82,0xFE,0xFE, // -]-
	0x20,0x40,0xFE,0x40,0x20, // -^-
	0x10,0x38,0x54,0x10,0x10, // -_-
	0x00,0x00,0x80,0x40,0x20, // -`-
	0x24,0x2A,0x2A,0x1C,0x02, // -a-
	0xFE,0x14,0x22,0x22,0x1C, // -b-
	0x1C,0x22,0x22,0x22,0x10, // -c-
	0x1C,0x22,0x22,0x14,0xFE, // -d-
	0x1C,0x2A,0x2A,0x2A,0x10, // -e-
	0x10,0x7E,0x90,0x90,0x40, // -f-
	0x19,0x25,0x25,0x25,0x1E, // -g-
	0xFE,0x10,0x20,0x20,0x1E, // -h-
	0x00,0x00,0x9E,0x00,0x00, // -i-
	0x00,0x01,0x11,0x9E,0x00, // -j-
	0xFE,0x08,0x14,0x22,0x02, // -k-
	0x00,0x82,0xFE,0x02,0x00, // -l-
	0x1E,0x20,0x1E,0x20,0x1E, // -m-
	0x20,0x1E,0x20,0x20,0x1E, // -n-
	0x1C,0x22,0x22,0x22,0x1C, // -o-
	0x3F,0x24,0x24,0x24,0x18, // -p-
	0x18,0x24,0x24,0x24,0x3F, // -q-
	0x20,0x1E,0x20,0x20,0x10, // -r-
	0x12,0x2A,0x2A,0x2A,0x24, // -s-
	0x20,0xFC,0x22,0x22,0x24, // -t-
	0x3C,0x02,0x02,0x3C,0x02, // -u-
	0x38,0x04,0x02,0x04,0x38, // -v-
	0x3C,0x02,0x3C,0x02,0x3C, // -w-
	0x22,0x14,0x08,0x14,0x22, // -x-
	0x39,0x05,0x05,0x09,0x3E, // -y-
	0x22,0x26,0x2A,0x32,0x22, // -z-
	0x10,0x6C,0x82,0x00,0x00, // -{-
	0x00,0x00,0xEE,0x00,0x00, // -|-
	0x00,0x00,0x82,0x6C,0x10, // -}-
	0x10,0x20,0x10,0x08,0x10, // -~-
	0xAA,0x54,0xAA,0x54,0xAA, // --
};

/*******************************************************************************
*函  数: *getASC(unsigned char ASC)                                            *
*功  能: 取点阵数据                                                            *
*参  数: 需要显示的Ascii                                                       *
*******************************************************************************//* 
unsigned char *getASC(unsigned char ASC)
{  //如果超过码表范围，一个空字符串作为安全返回 
   if ((ASC <32 ) || (ASC > 127)) { return AscLib[0]; } 
   return AscLib[(ASC-32)*5];
}
*/

/*******************************************************************************
* 函数名称: DlyMs()
* 功能描述: 延时函数 32MHz钟频下，约循环3000次延迟1毫秒
* 输入参数: 延时毫秒
* 返回参数: 无
********************************************************************************/
void DlyMs(int n)        
{
  int i,j;
  if(n<0)    { for(i=0;i< 30;i++); return; } //10us
  if(n== 0)  { for(i=0;i<150;i++); return; } //50us
  for(;n;n--){ for(j=0;j<3000;j++);        } //n毫秒
}

/*******************************************************************************
* 函数名称: PCA8538_Init()
* 功能描述: COG模块初始化
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void PCA8538_SPI_Init(void)
{       
	unsigned char i=0;
    cog_rst = 0;
	DlyMs(5);
    cog_rst = 1;
	DlyMs(5);

    cog_cs = 0;
	SPI_Sends(0X20); //SUBADRESS
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0X3A); //PCA8538 初始化

	DlyMs(5);
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0xD8); // OTP refresh
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0x18); // Device Select 0
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0xD4); // CLKOUT disabled
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0xC9); // Charge pump enabled, Vlcd = 3*Vdd2,max= 5*Vdd2
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0x45); // set VLCD, MSB
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0x73); // set VLCD, LSB. VLCD = 6.4V

	DlyMs(50);
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0x00); // Temp. comp. and measurement disabled
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0xD6); // Set 1/4 bias
	
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0xB4); // Driving scheme C, 3-line inversion
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0xd0); // 1:9
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0x39); // Display enable
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0x80); // Set Data pointer x-MSB = 0
	SPI_Sends(0X80); //控制字节
	SPI_Sends(0x90); // Set Data pointer x-LSB = 0
	SPI_Sends(0X80); //控制字节--最后一个命令
	SPI_Sends(0xA0); // Set Data pointer y = 0 	
				
	SPI_Sends(0X20); //向RAM写入数据 

	//清除显示
 	for(i=0;i<204;i++)  {SPI_Sends(0x00);} 

        DlyMs(200);
    cog_cs = 1;

} 

/*******************************************************************************
* 函数名称: clear_LCD_Dianzhen()
* 功能描述: 清除点阵区
* 输入参数: void
* 返回参数: 无
********************************************************************************/
void Clear_LCD_Dianzhen(void) 
{
    uchar i;
    cog_cs = 0;
	SPI_Sends(0x20);	
	SPI_Sends(0x80);
    SPI_Sends(0x80);
	SPI_Sends(0x80);
    SPI_Sends(0x90);
	SPI_Sends(0x80);
    SPI_Sends(0xA0);
	SPI_Sends(0x20);
	for(i=0;i<95;i++)
	{
		SPI_Sends(0x00);	
	}
    cog_cs = 1;	
	
}


/*******************************************************************************
*函  数:void PCA8538_WriteData(uchar XPointer, uchar YPointer, uchar Data)     *
*功  能:写点阵数字                                                             *
*参  数:                                                                       *
*******************************************************************************/  
void PCA8538_WriteData(unsigned char XPointer, unsigned char YPointer, unsigned char Data)
{
  unsigned char i;
  
  cog_cs = 0;
  
  SPI_Sends(0X20); //SUBADRESS
  SPI_Sends(0X80); //控制字节
  SPI_Sends((0x80 | ((XPointer >> 4) & 0x07))); // Set Data pointer x-MSB = 0
  
  SPI_Sends(0X80); //控制字节
  SPI_Sends((0x90 | (XPointer & 0x0F))); // Set Data pointer x-LSB = 0
  
  SPI_Sends(0X80); //控制字节--最后一个命令
  SPI_Sends((0xA0 | (YPointer & 0x01))); // Set Data pointer y = 0
  
  SPI_Sends(0X20); //SUBADRESS      
    
  //for(temp=0; temp<5; temp++) { SPI_Sends(*getASC(Data+temp)); }
  for (i=0; i<5; i++) {SPI_Sends(AscLib[(Data - 32) * 5 + i]);}

  SPI_Sends(0x00);

  cog_cs = 1;
}


/*******************************************************************************
*函  数: lcd_cls()                                                             *
*功  能: 清屏                                                                  *
*参  数: 无                                                                    *
*******************************************************************************/ 
void lcd_cls(void)        
{	
	Clear_LCD_Dianzhen();
        lcd_goto1(0);
} 

/*******************************************************************************
*函  数: LCD_SetXY(unsigned char x,unsigned char y)                            *
*功  能: 移动显示指针到指定位置                                                *
*参  数: X,Y  座标值                                                           *
*******************************************************************************/ 
void LCD_SetXY(unsigned char x,unsigned char y)
{
  cog_cs = 0;
  
  SPI_Sends(0X20); //SUBADRESS
  SPI_Sends(0X80); //控制字节
  SPI_Sends((0x80 | ((x >> 4) & 0x07))); // Set Data pointer x-MSB = 0
  
  SPI_Sends(0X80); //控制字节
  SPI_Sends((0x90 | (x & 0x0F))); // Set Data pointer x-LSB = 0
  
  SPI_Sends(0X80); //控制字节--最后一个命令
  SPI_Sends((0xA0 | (y & 0x01))); // Set Data pointer y = 0
  
  SPI_Sends(0X20); //SUBADRESS      
    
  cog_cs = 1; 
}

/*******************************************************************************
*函  数: lcd_goto1(unsigned char x)                                            *
*功  能: 移动到第一行的指定位置                                                *
*参  数: 字符在屏上的位置                                                      *
*******************************************************************************/ 
void lcd_goto1(unsigned char x){LCD_SetXY(0,x*6);curr_x=x*6;curr_y=0;} //第1行x位
//void lcd_goto2(uchar x){LCD_SetXY(16,x*8);curr_x=x*8;curr_y=16;} //第2行x位
//void lcd_goto3(uchar x){LCD_SetXY(32,x*8);curr_x=x*8;curr_y=32;} //第3行x位
//void lcd_goto4(uchar x){LCD_SetXY(48,x*8);curr_x=x*8;curr_y=48;} //第4行x位
void lcd_goto2(unsigned char x){curr_y=8;}
void lcd_goto3(unsigned char x){curr_y=16;}
void lcd_goto4(unsigned char x){curr_y=24;}

/*******************************************************************************
*函  数: lcd_putc(unsigned char d)                                             *
*功  能: 字符输出                                                              *
*参  数: 要输出显示的字符                                                      *
*******************************************************************************/ 
void lcd_putc(unsigned char d) 
{
	if (curr_y == 0)
	{
	PCA8538_WriteData(curr_x, curr_y, d);
	curr_x = curr_x + 6;
	}
}

/*******************************************************************************
*函  数: lcd_puts(unsigned char *s)                                            *
*功  能: 字串输出                                                              *
*参  数: 要输出显示的字串                                                      *
*******************************************************************************/
void lcd_puts(unsigned char *s) { for(; *s; s++){lcd_putc(*s);} } 

/*******************************************************************************
*函  数: lcd_init(void)                                                        *
*功  能: 初始化                                                                *
*参  数: 无                                                                    *
*******************************************************************************/
void lcd_init(void) { PCA8538_SPI_Init(); }


 
