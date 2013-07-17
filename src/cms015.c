
/***********************************************************************
*MCU stc12c5a60s2
*晶振 32MHz
*cms015 8 位数据线程序
*   端口名称              端口符号    LCD引脚　  MCU端口   
*   片选选择线               CS                   P3^6
*   RS数据/指令选择线        RS                   P3^7     0命令1数据
*   WR写选择线               WR                   P4^6     0写1读
*   RST写使能线              RST                  P4^5     使能位,下降沿触发
*   RD读选择线               RD                   P4^4
*   数据总线                 D0-D7     7-10       P0^0-P0^7
*
*   其他端口连接：
*   BOOT0                                                  接地
*   电源负                   VSS        1      
*   电源正+4.5--+5.5V        VDD        2
*   对比度调节 (接地最大)    VL         3 
*   背光负                   BLK VSS    15    
*   背光正                   BLA VDD    16
***********************************************************************/

#include <reg52.h>
#include "macros.h"
#include "delay.h"

/* LCDPort contains 8-bit data D0 to D7 */
#define LCDPORT         P0

/* Pins CS,RS,WR,RD,RST of LCD must be assigned to LCD Control Port*/
#define LCDCTRLP4       P4
#define LCDCTRLP3       P3

/* LCD Read pin is assigned to Px4 of P4 */
#define LCD_RD          4
/* LCD Write pin is assigned to Px5 of P4 */
#define LCD_WR          5
/* LCD Reset pin is assigned to Px6 of P4 */
#define LCD_RST         6

/* LCD RS pin is assigned to Px7 of P3 */
#define LCD_RS          7
/* LCD RS pin is assigned to Px6 of P3 */
#define LCD_CS          6

#define SET_RS		sbi(LCDCTRLP3, LCD_RS)
#define SET_CS          sbi(LCDCTRLP3, LCD_CS)
#define SET_RD 		sbi(LCDCTRLP4, LCD_RD)
#define SET_WR 		sbi(LCDCTRLP4, LCD_WR)
#define SET_RST         sbi(LCDCTRLP4, LCD_RST)

#define CLR_RS		cbi(LCDCTRLP3, LCD_RS)
#define CLR_CS          cbi(LCDCTRLP3, LCD_CS)
#define CLR_RD 		cbi(LCDCTRLP4, LCD_RD)
#define CLR_WR 		cbi(LCDCTRLP4, LCD_WR)
#define CLR_RST         cbi(LCDCTRLP4, LCD_RST)


void WriteCOM(unsigned char  b)
{
	u16 ud;
	ud=b*256;
	CLR_RS;
	CLR_CS;
	SET_RD;
	CLR_WR;
	cbi(LCDPORT, (0xff00));
	SET_WR;

	CLR_WR;
	sbi(LCDPORT, ud & 0xff00);
	cbi(LCDPORT, (~ud & 0xff00));
	SET_WR;
	SET_RS;
	SET_CS;
}

void WriteDAT(u8 a,u8 b) //写16位数据
{
	u16 ud;
	ud=a*256;

   	SET_RS; 
   	CLR_CS; 
	SET_RD;

   	CLR_WR;
	sbi(LCDPORT, ud & 0xff00);
	cbi(LCDPORT, (~ud & 0xff00));
	SET_WR;

	ud=b*256;
	CLR_WR;
	sbi(LCDPORT, ud & 0xff00);
	cbi(LCDPORT, (~ud & 0xff00));
	SET_WR;

  	CLR_RS;
   	SET_CS;
      
}

void WriteDAT8(unsigned char b)     //写8位数据
{
	u16 ud;
	ud=b*256;

   	SET_RS; 
   	CLR_CS; 
	SET_RD;
   
	sbi(LCDPORT, ud & 0xff00);
	cbi(LCDPORT, (~ud & 0xff00));
	CLR_WR;
	SET_WR;

   	CLR_RS;
   	SET_CS;
}

void LCD_CMD(u16 cmd,u16 dat)
{	u8 i,j;
	i=cmd &0xff;
	WriteCOM(i);
	i=dat>>8;
	j=dat&0xff;
	WriteDAT(i ,j);
}

void LCD_SetXY(u8 x,u8 y)
{	x=219-x;
	y=175-y;
	LCD_CMD(0x0021,y+x*256);
	WriteCOM(0x22);   
}

void LCD_INIT()
{  	u16 i;
	SET_CS;
	SET_RST;
	DelayMs(200);
	CLR_RST;
	DelayMs(200);
	SET_RST;
	DelayMs(200);
 
	LCD_CMD(0x0001,0x001B);   
    	LCD_CMD(0x0002,0x0400);    
    	LCD_CMD(0x0003,0x6000); //6020    
    	LCD_CMD(0x0004,0x0000);   
    	LCD_CMD(0x0005,0x0000);   
    	LCD_CMD(0x0008,0x0202);   
    	LCD_CMD(0x0009,0x0000);   
    	LCD_CMD(0x000B,0x0000);   
    	LCD_CMD(0x000C,0x0003);   
    	LCD_CMD(0x0040,0x0000);   
    	LCD_CMD(0x0041,0x00ef); //00ef    
    	LCD_CMD(0x0042,0xDB00);   
    	LCD_CMD(0x0043,0xDB00);   
    	LCD_CMD(0x0021,0x0000);   
    	LCD_CMD(0x0044,0xAF00);   
    	LCD_CMD(0x0045,0xDB00);
    	LCD_CMD(0x0001,0x001B);   
    	LCD_CMD(0x0002,0x0400);    
    	LCD_CMD(0x0003,0x1000); //6020    
   	LCD_CMD(0x0004,0x0000);   
    	LCD_CMD(0x0005,0x0000);   
    	LCD_CMD(0x0007,0x0004);   
    	LCD_CMD(0x0008,0x0202);   
    	LCD_CMD(0x0009,0x0000);   
    	LCD_CMD(0x000B,0x0000);   
    	LCD_CMD(0x000C,0x0003);   
    	LCD_CMD(0x0040,0x0000);   
    	LCD_CMD(0x0041,0x00ef); //00ef    
    	LCD_CMD(0x0042,0xDB00);   
    	LCD_CMD(0x0043,0xDB00);   
    	LCD_CMD(0x0021,0x0000);   
    	LCD_CMD(0x0044,0xAF00);   
    	LCD_CMD(0x0045,0xDB00);   
    	DelayMs(50); // Delay 50ms   

    	LCD_CMD(0x0000,0x0001);   
    	DelayMs(50); // Delay 50ms   

    	LCD_CMD(0x0010,0x0000);   
    	LCD_CMD(0x0011,0x0000);   
    	LCD_CMD(0x0012,0x0000);    
    	LCD_CMD(0x0013,0x0000);    
    	LCD_CMD(0x0013,0x1517);   
    	LCD_CMD(0x0012,0x0008);   
    	LCD_CMD(0x0010,0x4040);   
    	DelayMs(50); // Delay 50ms 

    	LCD_CMD(0x0010,0x4140);   
    	LCD_CMD(0x0012,0x0012);   
    	DelayMs(50); // Delay 50ms   

    	LCD_CMD(0x0010,0x4140);   
    	DelayMs(50); // Delay 50ms   

    	LCD_CMD(0x0013,0x3308);   
    	DelayMs(50); // Delay 50ms          

    	LCD_CMD(0x0030,0x0000);   
    	LCD_CMD(0x0031,0x0207);   
    	LCD_CMD(0x0032,0x0002);   
    	LCD_CMD(0x0033,0x0104);   
    	LCD_CMD(0x0034,0x0305);   
    	LCD_CMD(0x0035,0x0002);   
    	LCD_CMD(0x0036,0x0706);   
    	LCD_CMD(0x0037,0x0503);   
    	LCD_CMD(0x0038,0x0000);   
    	LCD_CMD(0x0039,0x0000);   
    	LCD_CMD(0x0007,0x0005);   
    	DelayMs(50); // Delay 50ms  

    	LCD_CMD(0x0007,0x0025);   
    	LCD_CMD(0x0007,0x0027);   
    	DelayMs(50); // Delay 50ms 

    	LCD_CMD(0x0007,0x0037);    
    	DelayMs(50); // Delay 50ms  

    	LCD_CMD(0x0021,0x0000);//RAM address set    
    	WriteCOM(0x22);//RAM address set     
	
	for(i=0;i<38720;i++) { WriteDAT(0x00,0x00); }
}






void LCDTEST(void)
{	  	
	u16 p,q;
	u8 w=22;
    	LCD_CMD(0x0021,0x0000);//RAM address set    
    	WriteCOM(0x22);//RAM address set     

	for(p=0;p<200;p++)
	{
		for(q=0;q<w;q++) { WriteDAT(0xf8,0x00); }
		for(q=0;q<w;q++) { WriteDAT(0x07,0xe0); }
		for(q=0;q<w;q++) { WriteDAT(0x00,0x1f); }
		for(q=0;q<w;q++) { WriteDAT(0xff,0xff); }
		for(q=0;q<w;q++) { WriteDAT(0x00,0x00); }
		for(q=0;q<w;q++) { WriteDAT(0x00,0x1f); }		
		for(q=0;q<w;q++) { WriteDAT(0x07,0xe0); }
		for(q=0;q<w;q++) { WriteDAT(0xf8,0x00); }
	}

}




