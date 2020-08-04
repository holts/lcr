#include "n1110.h"
#include "lcd_generic_font.h"

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "core_cmInstr.h"




char mask;
int xpos;

int vmirror = 0;

/***************** LOW LEVEL ************************************************/
void delayus(int us)
{
	us *= 4;
	for(;us>0;us--) __NOP();
}

void __attribute__ ((noinline))  lcd_init (int mode)
{

	LCD_GPIO->BSRR = ((LCD_SCK_PIN | LCD_SDA_PIN)<<16) |  ( LCD_RESET_PIN | LCD_CS_PIN );
	//LCD_GPIO->BRR = ;

	lcd_hw_reset(); // HW reset


	lcd_write(COMMAND, 0xE2); // Internal reset
	delayus(2000);
	lcd_write(COMMAND, 0xEB); // Thermal comp. on
	delayus(2000);
	lcd_write(COMMAND, 0x2F); // Supply mode

// default 0 is 1110
	if(! (mode & 1) ){  lcd_write(COMMAND, 0xC8);}

	if(mode & 2) lcd_write(COMMAND, 0xA1); else lcd_write(COMMAND, 0xA9);

	lcd_write(COMMAND, 0xA4); // Clear screen
	lcd_write(COMMAND, 0xA6); // Positive - A7, Negative - A6
	lcd_write(COMMAND, 0x91); // Contrast 0x80...0x9F

	lcd_write(COMMAND, 0xAF); // Enable LCD

	xpos = 0;

	lcd_clear();
}

/*
 * N1110 LCD hardware reset
 * !!!NOTE: needs normal delay implementation
 */
void __attribute__ ((noinline))  lcd_hw_reset (void){
	LCD_GPIO->BRR = LCD_RESET_PIN;
	delayus(10000);
	LCD_GPIO->BSRR = LCD_RESET_PIN;
	delayus(2000);
}

/*
 * N1110 LCD write data or command
 */
void __attribute__ ((noinline))  lcd_write (lcd_cd_t cd, uint8_t byte){
	register uint32_t i;
	uint32_t b  = byte ;

	xpos++;
	/* Slave select */
	LCD_GPIO->BRR = LCD_CS_PIN;

	if(vmirror && (cd ==DATA) ) b = __RBIT((uint32_t)b<<24);
	if(cd == DATA) b |= 1<<8;


	for(i=0; i<9; i++)
	{
		if(b & 0x100) LCD_GPIO->BSRR = ( LCD_SCK_PIN<<16 ) | LCD_SDA_PIN; //RESET SCK + SET SDA
		else 		LCD_GPIO->BSRR = ( (LCD_SDA_PIN | LCD_SCK_PIN )<<16);	//RESET SCK + SDA
		delayus(0);delayus(0);delayus(0);
		LCD_GPIO->BSRR = LCD_SCK_PIN;
		b <<= 1;
	}
	delayus(0);delayus(0);
	/* Slave release */
	LCD_GPIO->BSRR = ( (LCD_SDA_PIN | LCD_SCK_PIN )<<16 ) | LCD_CS_PIN; //RESET SCK + SDA set CS
}

/*
 * Clear LCD screen
 */
void __attribute__ ((noinline))  lcd_clear (void){

	lcd_gotoxy(0,0);
	for(int i=0; i < LCD_YMAX*LCD_XMAX; i++) lcd_write(DATA, 0x00);

}

/*
 * Set current position
 */
void __attribute__ ((noinline))  lcd_gotoxy (uint8_t x ,uint8_t y){
	xpos = x;

	lcd_write(COMMAND, (0xB0 | (y & 0x0F)) );
	lcd_write(COMMAND, (0x10 | (x >> 0x04)) );
	lcd_write(COMMAND, 0x0F & x);
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

				lcd_write(DATA,  mask);

				for(j=numbers_idx[n]+i; j< numbers_idx[n]+13*3 ; j+=3)
					{
					int dd = 0;
					if(j< numbers_idx[n+1]) dd = numbers[j];

					if( (*str2 == '.') && (i==0)&&(j>( numbers_idx[n]+13*3 - 9))) dd |= 0x06;

					if (xpos <= LCD_XMAX) lcd_write(DATA, dd ^ mask);

					}
			}
		}

	}


	//lcd_write(DATA,0);
}

/*
 * Put character to current position
 */
void __attribute__ ((noinline))  lcd_putchar (const char c){
	register uint32_t i;
	char cc = c;
	if(cc < 32) cc = 0; else cc -= 32;


	for(i=0; i<5; i++) if (xpos < LCD_XMAX) {lcd_write(DATA, (lcd_font[((cc&0x7f)*5)+i])^ mask);}

	if (xpos <= LCD_XMAX) lcd_write(DATA, 0x00 ^ mask);
}

/*
 * Put string from RAM
 */
void __attribute__ ((noinline))  lcd_putstr (const char *str,int fill ){
	char c;
	while( (c = (*str++))  ) lcd_putchar(c);

	if(fill){
	while(xpos <= LCD_XMAX) lcd_write(DATA, mask);
	}
}

void lcd_setcontrast(int c)
{
lcd_write(COMMAND, 0x80 |  (c&0x1F));
}
