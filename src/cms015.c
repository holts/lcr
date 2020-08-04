#include "cms015.h"
#include "lcd_generic_font.h"

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "core_cmInstr.h"

char mask;
int xpos;

int vmirror = 0;

/***************** LOW LEVEL ****************************************/
void delayus(int us)
{
	us *= 4;
	for(;us>0;us--) __NOP();
}


void  __attribute__ ((noinline)) WriteCMD(uint8_t cd)
{
	uint16_t ud;
	ud=cd*256;
	RS_0;
	CS_0;
	RD_1;
	WR_0;
	GPIO_ResetBits(LCD_GPIO, (0xff00));
	WR_1;

	WR_0;
	GPIO_SetBits(LCD_GPIO, ud & 0xff00);
	GPIO_ResetBits(LCD_GPIO, (~ud & 0xff00));
	WR_1;
	RS_1;
	CS_1;
}


void  __attribute__ ((noinline)) WriteDAT(uint8_t b)
{
	uint16_t ud;
	ud=b*256;
	RS_1;
	CS_0;
	RD_1;
    GPIO_SetBits(LCD_GPIO, ud & 0xff00);
	GPIO_ResetBits(LCD_GPIO, (~ud & 0xff00));
	WR_0;
	WR_1;
	RS_0;
	CS_1;
}


void __attribute__ ((noinline))  lcd_init (int mode)
{
    CS_1;
	lcd_hw_reset(); // HW reset

	lcd_write_cmd(0x0001,0x001B);   
    lcd_write_cmd(0x0002,0x0400);    
    lcd_write_cmd(0x0003,0x6000); //6020    
    
    lcd_write_cmd(0x0004,0x0000);   
    lcd_write_cmd(0x0005,0x0000);   
    //lcd_write_cmd(0x0007,0x0004);    
    lcd_write_cmd(0x0008,0x0202);   
    lcd_write_cmd(0x0009,0x0000);   
    lcd_write_cmd(0x000B,0x0000);   
    lcd_write_cmd(0x000C,0x0003);   
       
    lcd_write_cmd(0x0040,0x0000);   
    lcd_write_cmd(0x0041,0x00ef); //00ef    
    lcd_write_cmd(0x0042,0xDB00);   
    lcd_write_cmd(0x0043,0xDB00);   
       
    lcd_write_cmd(0x0021,0x0000);   
//    lcd_write_cmd(0x0023,0x0000);   
//    lcd_write_cmd(0x0024,0x0000);   
    lcd_write_cmd(0x0044,0xAF00);   
    lcd_write_cmd(0x0045,0xDB00);
        //other register setting    
    lcd_write_cmd(0x0001,0x001B);   
       
    lcd_write_cmd(0x0002,0x0400);    

//  lcd_write_cmd(0x0003,0x1038); //6020    
    lcd_write_cmd(0x0003,0x1000); //6020    
    lcd_write_cmd(0x0004,0x0000);   
    lcd_write_cmd(0x0005,0x0000);   
    lcd_write_cmd(0x0007,0x0004);   
    lcd_write_cmd(0x0008,0x0202);   
    lcd_write_cmd(0x0009,0x0000);   
    lcd_write_cmd(0x000B,0x0000);   
    lcd_write_cmd(0x000C,0x0003);   


    lcd_write_cmd(0x0040,0x0000);   
    lcd_write_cmd(0x0041,0x00ef); //00ef    
    lcd_write_cmd(0x0042,0xDB00);   
    lcd_write_cmd(0x0043,0xDB00);   
    lcd_write_cmd(0x0021,0x0000);   
//    lcd_write_cmd(0x0023,0x0000);   
//    lcd_write_cmd(0x0024,0x0000);   
	lcd_write_cmd(0x0044,0xAF00);   
    lcd_write_cmd(0x0045,0xDB00);   
	//other register setting    
    // Delay 50ms   
    delayus(50000);
    lcd_write_cmd(0x0000,0x0001);   
   
    // Delay 50ms   
    delayus(50000);
    lcd_write_cmd(0x0010,0x0000);   
    lcd_write_cmd(0x0011,0x0000);   
    lcd_write_cmd(0x0012,0x0000);    
    lcd_write_cmd(0x0013,0x0000);    
       
    lcd_write_cmd(0x0013,0x1517);   
    lcd_write_cmd(0x0012,0x0008);   
    lcd_write_cmd(0x0010,0x4040);   
    // Delay 50ms   
    delayus(50000);
    lcd_write_cmd(0x0010,0x4140);   
    lcd_write_cmd(0x0012,0x0012);   
    // Delay 50ms   
    delayus(50000);
    lcd_write_cmd(0x0010,0x4140);   
    // Delay 50ms   
    delayus(50000);
    //lcd_write_cmd(0x0013,0x3613);    
    lcd_write_cmd(0x0013,0x3308);   
       
    // Delay 50ms   
    delayus(50000);
    //gamma CORRECTION    
    lcd_write_cmd(0x0030,0x0000);   
    lcd_write_cmd(0x0031,0x0207);   
    lcd_write_cmd(0x0032,0x0002);   
    lcd_write_cmd(0x0033,0x0104);   
    lcd_write_cmd(0x0034,0x0305);   
    lcd_write_cmd(0x0035,0x0002);   
    lcd_write_cmd(0x0036,0x0706);   
    lcd_write_cmd(0x0037,0x0503);   
    lcd_write_cmd(0x0038,0x0000);   
    lcd_write_cmd(0x0039,0x0000);   
    //gamma CORRECTION    
       
    lcd_write_cmd(0x0007,0x0005);   
    // Delay 50ms   
    delayus(50000);
    lcd_write_cmd(0x0007,0x0025);   
    lcd_write_cmd(0x0007,0x0027);   
    // Delay 50ms   
    delayus(50000);
    lcd_write_cmd(0x0007,0x0037);    
    // Delay 50ms   
    delayus(50000);

    lcd_write_cmd(0x0021,0x0000);//RAM address set    
    WriteCMD(0x22);//RAM address set     
	
	lcd_clear();
}

/*
 * LCD hardware reset
 * !!!NOTE: needs normal delay implementation
 */
void __attribute__ ((noinline))  lcd_hw_reset (void){
	LCD_GPIO->BRR = LCD_RESET_PIN;
	delayus(10000); // 10ms
	LCD_GPIO->BSRR = LCD_RESET_PIN;
	delayus(2000);  // 2ms
}


/*
 * cms015 LCD write command
 */
void __attribute__ ((noinline))  lcd_write_cmd (uint16_t cmd, uint16_t dat){
	uint8_t i,j;

    i = cmd &0xff;
    WriteCMD(i);
    i = dat>>8;
    j = dat&0xff;
    lcd_write_data(i,j);

    //xpos++;
}


/*
 * cms015 LCD write data
 */
void __attribute__ ((noinline)) lcd_write_data (uint8_t a, uint8_t b){
	uint16_t ud = a*256;
    //xpos++;

   	RS_1; 
   	CS_0; 
	RD_1;

   	WR_0;
	GPIO_SetBits(LCD_GPIO, ud & 0xff00);
	GPIO_ResetBits(LCD_GPIO, (~ud & 0xff00));
	WR_1;

	ud = b*256;
	WR_0;
	GPIO_SetBits(LCD_GPIO, ud & 0xff00);
	GPIO_ResetBits(LCD_GPIO, (~ud & 0xff00));
	WR_1;

  	RS_0;
   	CS_1;
}


/*
 * Clear LCD screen
 */
void __attribute__ ((noinline))  lcd_clear (void){

	lcd_gotoxy(0,0);
	for(int i=0; i < LCD_YMAX*LCD_XMAX; i++) lcd_write_data(0x00, 0x00);

}

/*
 * Set current position
 */
void __attribute__ ((noinline))  lcd_gotoxy (uint8_t x, uint8_t y){
	xpos = x;

    x = 219 - x;
    y = 175 - y;
    lcd_write_cmd(0x0021, y+x*256);
    WriteCMD(0x22);
}

void  lcd_putnum (int x, int y,char *str){

	char c;
	int i,j;

	char* str2 = str;


	for(i =0;i<3;i++)
	{
		lcd_gotoxy(x,y+2-i);

		str2 = str;
		while( (c = (*str2++)) )
		{
			if( (c>=0x30  &&  c<= 0x39)|| (c<11) )
			{
				int n = c - 0x30;
				if(n < 0) n = c+10;

				WriteDAT( mask);

				for(j=numbers_idx[n]+i; j< numbers_idx[n]+13*3 ; j+=3)
					{
					int dd = 0;
					if(j< numbers_idx[n+1]) dd = numbers[j];

					if( (*str2 == '.') && (i==0)&&(j>( numbers_idx[n]+13*3 - 9))) dd |= 0x06;

					if (xpos <= LCD_XMAX) WriteDAT( dd ^ mask);

					}
			}
		}
	}
}

/*
 * Put character to current position
 */
void __attribute__ ((noinline))  lcd_putchar (const char c){
	register uint32_t i;
	char cc = c;

	if(cc < 32) cc = 0; else cc -= 32;

	for(i=0; i<5; i++) 
        if (xpos < LCD_XMAX) WriteDAT( (lcd_font[((cc&0x7f)*5)+i])^ mask);

	if (xpos <= LCD_XMAX) WriteDAT( 0x00 ^ mask);
}

/*
 * Put string from RAM
 */
void __attribute__ ((noinline))  lcd_putstr (const char *str,int fill ){
	char c;
	while( (c = (*str++))  ) lcd_putchar(c);

	if(fill){
	while(xpos <= LCD_XMAX) WriteDAT( mask);
	}
}

void lcd_setcontrast(int c)
{ WriteCMD( 0x80 |  (c&0x1F)); }
