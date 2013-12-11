
#include "stdafx.h"
#include "..\LCD.h"
#include "..\LCDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "lcd_driver_KS0107.h"
//#include <util/delay.h>
//#include <avr/pgmspace.h>
//#include <avr/io.h>
#include "font5x7.h"


extern  unsigned char  DDRAM[X_MAX*Y_MAX];
//------------------------------------------------------------------------------
//Global variables used by driver
uint8_t LCD_currentX, LCD_currentY;

static void M_delayus( void )
{
	::Sleep(10);// ÐÝÃß
}


uint8_t  pgm_read_byte( uint8_t *data )
{
	return  *data;
}


//------------------------------------------------------------------------------
//Write a byte to LCD.  Single 8 bit segment is send to current cursor position
//
void LCD_writeByte(uint8_t data)
{
	int  x,y;
	uint8_t mask=0x01; 

	//LCD_setCursorXY(LCD_currentX,LCD_currentY); //address and chip selected 

	x = LCD_currentX;
	y = LCD_currentY;

	for(;y<8+LCD_currentY;y++)
	{
		if((data&mask) != 0x00)
			DDRAM[y*X_MAX+x] = 0xff;
		mask = mask << 1;
	}	
	LCD_currentX ++;
	LCD_setCursorXY(LCD_currentX,LCD_currentY);
}

//------------------------------------------------------------------------------
//Move cursor to position x y
void LCD_setCursorXY(uint8_t x, uint8_t y)
{
  LCD_currentX=x;
  LCD_currentY=y;
}


//------------------------------------------------------------------------------
//clear LCD DDRAM 
void LCD_clr(void)
{
	int  i,j;

	for(i=0;i<Y_MAX;i++)
	{
		for(j=0;j<X_MAX;j++)
		{
			DDRAM[i*X_MAX+j] = 0x00;
		}
	}
}


//------------------------------------------------------------------------------
//Draw pixel at x y
void LCD_pixelOn(uint8_t x, uint8_t y)
{  
	DDRAM[y*X_MAX+x] = 0xff;
}
//------------------------------------------------------------------------------
//Dim pixel at x y
void LCD_pixelOff(uint8_t x, uint8_t y)
{
	DDRAM[y*X_MAX+x] = 0x00;
}
//------------------------------------------------------------------------------
//Invert pixel value at x y
void LCD_invertPixel(uint8_t x, uint8_t y)
{
	if(DDRAM[y*X_MAX+x] == 0xff)
		DDRAM[y*X_MAX+x] = 0x00;
	else
		DDRAM[y*X_MAX+x] = 0xff;
}
//------------------------------------------------------------------------------
//invert pixels on a page from x1 to x2
// x1 must be smaller than x2
void LCD_invertPage(uint8_t page, uint8_t x1, uint8_t x2)
{
  uint8_t  mask,i;
  uint8_t  y;

    for (y=page*LCD_PAGE_HEIGHT;y<=(page+1)*LCD_PAGE_HEIGHT;y++)
	{
		  for (i=x1;i<=x2;i++)
		  {
			mask = DDRAM[y*X_MAX+i];
			DDRAM[y*X_MAX+i] = ~mask;
		  }
	}
}

//------------------------------------------------------------------------------
//clear a single page in LCD DDRAM from x1 to x2
void LCD_offPage(uint8_t p,uint8_t x1, uint8_t x2)
{
  uint8_t  i;
  uint8_t  y;

    for (y=p*LCD_PAGE_HEIGHT;y<=(p+1)*LCD_PAGE_HEIGHT;y++)
	{
		  for (i=x1;i<=x2;i++)
		  {
			DDRAM[y*X_MAX+i] = 0x00;
		  }
	}
}
//------------------------------------------------------------------------------
//Turn pixels on in a single page in LCD DDRAM from x1 to x2
void LCD_onPage(uint8_t p,uint8_t x1, uint8_t x2)
{
  uint8_t  i;
  uint8_t  y;

    for (y=p*LCD_PAGE_HEIGHT;y<=(p+1)*LCD_PAGE_HEIGHT;y++)
	{
		  for (i=x1;i<=x2;i++)
		  {
			DDRAM[y*X_MAX+i] = 0xff;
		  }
	}
}

//------------------------------------------------------------------------------
//Put a single char to LCD on line at current cursor position
// Note that page is used not exact coordinates 
void LCD_putchar(uint8_t c)
{
	uint8_t i = 0;  

	for(i=0; i<5; i++)
	{         
		if (LCD_currentX>=LCD_LINE_LENGTH)
		{
			if (LCD_currentY<LCD_LINES+LCD_PAGE_HEIGHT)
			{
				LCD_setCursorXY(0,LCD_currentY+LCD_PAGE_HEIGHT);
			}
			else {
				LCD_setCursorXY(0,0);	  
			}
		}
		LCD_writeByte(pgm_read_byte(&Font5x7[(c - 0x20)*5 + i])); 
	}  
	
	LCD_writeByte(0x00);  
}

//---------------------------------------------------------------------------------
//Put a string on display - auto for next line is default (from LCD_putchar)
//null terminated strings are expected
// /n  for new line 
void LCD_puts(uint8_t* pString)
{    	
	uint8_t i=0;

	while (pString[i] != '\0')
	{
		if (pString[i]=='\n')
		{           
			LCD_currentX=0;
			LCD_currentY+=LCD_PAGE_HEIGHT;    
		}
		else {
			LCD_putchar(pString[i]);
		}
		i++;      
	}
}

//-----------------------------------------------------------------------------
//Put a string on selected page (line) starting at x
//
void LCD_putsp(uint8_t* pString, uint8_t page, uint8_t x)
{
  LCD_currentX=x;
  LCD_currentY=page*LCD_PAGE_HEIGHT;

  LCD_puts(pString);
}






 
