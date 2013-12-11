
#include "stdafx.h"
#include "..\LCD.h"
#include "..\LCDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#include <avr/pgmspace.h>
#include "lcd_graphics.h"

//select suitable driver 
//#include "lcd_driver_S6B0724.h"
//#include "lcd_driver_SED1520.h"
#include "lcd_driver_KS0107.h"

extern uint8_t LCD_currentX; //from driver
extern uint8_t LCD_currentY;

//------------------------------------------------------------------------------
//Draw a line from x to y 
void LCD_drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  _LCD_doLine(x0,y0,x1,y1,&LCD_pixelOn);
}

//------------------------------------------------------------------------------
//Draw a line from x to y 
void LCD_clearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  _LCD_doLine(x0,y0,x1,y1,&LCD_pixelOff);
}

//------------------------------------------------------------------------------
//Draw a line from x to y 
void LCD_invertLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  _LCD_doLine(x0,y0,x1,y1,&LCD_invertPixel);
}

//------------------------------------------------------------------------------
//Draw a rectangle 
void LCD_drawRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
  LCD_drawLine(x1,y1, x2,y1);
  LCD_drawLine(x1,y1, x1,y2);
  LCD_drawLine(x2,y2, x2,y1);
  LCD_drawLine(x1,y2, x2,y2);  
}

//------------------------------------------------------------------------------
//Draw a circle
void LCD_drawCirc(uint8_t x1, uint8_t y1, uint8_t radius)
{
  uint8_t y=0, x=0, d = 0;
  int8_t part; 
  d = y1 - x1;
  y = radius;
  part = 3 - 2 * radius;
  while (x <= y) { 
    LCD_pixelOn(x1 + x, y1 + y);  
    LCD_pixelOn(x1 + x, y1 - y);
    LCD_pixelOn(x1 - x, y1 + y);    
    LCD_pixelOn(x1 - x, y1 - y);
    LCD_pixelOn(y1 + y - d, y1 + x); 
    LCD_pixelOn(y1 + y - d, y1 - x);
    LCD_pixelOn(y1 - y - d, y1 + x);
    LCD_pixelOn(y1 - y - d, y1 - x); 
    if (part < 0) part += (4 * x + 6);
    else {
      part += (4 * (x - y) + 10);
      y--;
    }
    x++;
  }
}

//------------------------------------------------------------------------------
//Draw a filled circle
void LCD_fillCirc(uint8_t x1, uint8_t y1, uint8_t radius)
{  
  _LCD_doCirc(x1,y1,radius,&LCD_drawLine);
}

//------------------------------------------------------------------------------
//Clear area of circle
void LCD_clearCirc(uint8_t x1, uint8_t y1, uint8_t radius)
{  
  _LCD_doCirc(x1,y1,radius,&LCD_clearLine);
}

//------------------------------------------------------------------------------
//Puts a bit map to LCD
void LCD_drawFullBMP(uint8_t* bitmap)
{  
  uint8_t page,j;  
  for (page=0;page<LCD_LINES/LCD_PAGE_HEIGHT;page++){      
    LCD_setCursorXY(0,page*LCD_PAGE_HEIGHT);
    for (j=0;j<LCD_LINE_LENGTH;j++){              
      LCD_writeByte(pgm_read_byte(&bitmap[page*LCD_LINE_LENGTH+j]));
    }
  }
  
}
//------------------------------------------------------------------------------
// Invert rectangle 
//
void LCD_invertRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
  _LCD_doRect(x1,y1,x2,y2, &LCD_invertPixel, &LCD_invertPage);
}

//------------------------------------------------------------------------------
void LCD_clearRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
  _LCD_doRect(x1,y1,x2,y2, &LCD_pixelOff, &LCD_offPage);
}

void LCD_fillRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
  _LCD_doRect(x1,y1,x2,y2, &LCD_pixelOn, &LCD_onPage);
}

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//******************************************************************************
//Help functions NOT ment for API use

//------------------------------------------------------------------------------
uint8_t _LCD_absDiff(uint8_t a,uint8_t b){ if(a>b) return a-b; else return b-a;}

//------------------------------------------------------------------------------
//general function to do rectangle operations 
// (uses function pointers to pixel and page operations provided by driver)
// call eg.  _LCD_doRect(20,20,107,43, &LCD_invertPixel, &LCD_invertPage);
//  
void _LCD_doRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2, 
		 void (*pPixFun)(uint8_t, uint8_t ),
		 void (*pPageFun)(uint8_t, uint8_t, uint8_t )  
		)
{
  uint8_t i,j,lines, page, _x1, _x2, _y1, _y2;
  if(x1 > x2) {_x1=x2; _x2=x1; _y1=y2; _y2=y1;}  
  else {_x1=x1; _x2=x2; _y1=y1; _y2=y2;}  
  if(_y1>_y2) {i=_y1, _y1=_y2, _y2=i;}
  
  //not whole page lines in the beginning
  lines=( (y1/LCD_PAGE_HEIGHT+1)*LCD_PAGE_HEIGHT-_y1)%LCD_PAGE_HEIGHT;
  for(i=_y1;i<_y1+lines;i++)
    for(j=_x1;j<=_x2;j++)
      pPixFun(j,i);
  
  page= i/LCD_PAGE_HEIGHT;    
  lines=(_y2-i)/LCD_PAGE_HEIGHT; //whole page lines left  
  for(j=0;j<lines;j++){
    pPageFun(page++,_x1,_x2);
  }
  
  i+=lines*LCD_PAGE_HEIGHT;
  while(i!=_y2){                 //last lines
    for(j=_x1;j<=_x2;j++)
      pPixFun(j,i);
    i++;
  }
}


//------------------------------------------------------------------------------
//General line operation from x to y using  Bresenham's line algorithm
//ref: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
// pPixFun must point to function that operates on single pixel
// call eg.  _LCD_doLine(0,0,127,63, &LCD_invertPixel);
//
void _LCD_doLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
		void (*pPixFun)(uint8_t, uint8_t ))
{
  uint8_t dx, dy, i,j, _x1, _y1, _x0, _y0, is_steep;
  int8_t error, ystep;
  is_steep = _LCD_absDiff(y0,y1) > _LCD_absDiff(x0,x1);  

  //order the init points ascending
  if ( is_steep ){_x0=y0; _y0=x0; _x1=y1; _y1=x1; }
  else {_x0=x0; _x1=x1; _y0=y0; _y1=y1;  }
  if (_x0 > _x1) {i=_x0; _x0=_x1; _x1=i; i=_y0; _y0=_y1; _y1=i;}

  //check boundaries
  if (_x0>=LCD_LINE_LENGTH) _x0=0;
  if (_x1>=LCD_LINE_LENGTH) _x1=0;
  if (_y0>=LCD_LINE_LENGTH) _y0=0;
  if (_y1>=LCD_LINE_LENGTH) _y1=0;
  
  dx= _x1 - _x0;
  dy=_LCD_absDiff(_y0,_y1);  
  error = -(dx+1)/2;
  j=_y0;
  if (_y0<_y1) ystep=1;  else ystep=-1;

  for(i=_x0;i<=_x1;i++){
    if (is_steep) pPixFun(j,i);
    else pPixFun(i,j);
    error += dy;
    if (error>0){
      j=j+ystep;
      error=error-dx;
    }
  }
}

//------------------------------------------------------------------------------
//General circle operation (!NON OPTIMIZED = same pixels drawn multiple times)
// pLineFun must point to function that draws lines between points
// call eg.  _LCD_doLine(0,0,127,63, &LCD_invertPixel);
void _LCD_doCirc(uint8_t x1, uint8_t y1, uint8_t radius,
		 void (*pLineFun)(uint8_t,uint8_t,uint8_t,uint8_t))
{  
  uint8_t  y=0, x=0, d = 0;
  int8_t part; 
  d = y1 - x1;
  y = radius;
  part = 3 - 2 * radius;
  while (x <= y) {
    pLineFun(x1 + x, y1 + y,x1 + x, y1 - y);
    pLineFun(x1 - x, y1 + y,x1 - x, y1 - y);
    pLineFun((y1 + y - d), y1 + x,(y1 + y - d), y1 - x);
    pLineFun((y1 - y - d), y1 + x,(y1 - y - d), y1 - x);
    if (part < 0) part += (4 * x + 6);
    else {
      part += (4 * (x - y) + 10);
      y--;
    }
    x++;
  }
}

//-----------------------------------------------------------------------------
// put a graphical bitmap with width and height at x,y 
//
void LCD_drawBMP(uint8_t* bitmap, uint8_t x, uint8_t y, 
		   uint8_t width, uint8_t height)
{
  uint8_t pages,i,j;
  //whole pages  
  pages=height/LCD_PAGE_HEIGHT;
  j=0;
  while(j<pages){
    LCD_copyPageTo(bitmap, width, j, 0, width, x, y+(j*LCD_PAGE_HEIGHT));
    j++;
  }
  //single lines left      
  //  single_lines=height%LCD_PAGE_HEIGHT;
  j*=LCD_PAGE_HEIGHT;
  while (j < height){
    for(i=0;i<width;i++)
      LCD_copyPixelTo(bitmap, width, i, j, x+i, y+j);  
    j++;
  }

  LCD_currentX=x+width;
  LCD_currentY=y;

  if(LCD_currentX>=LCD_LINE_LENGTH) {
    LCD_currentX=0;
    LCD_currentY+=height;
    if(LCD_currentY>=LCD_LINES) LCD_currentY=0;
  }

}

//----------------------------------------------------------------------------
//Copy a single pixel to LCD (tx,ty) from source (sx,sy) 
//Source must be a 8-bit paged bitmap image in pgm memory
void LCD_copyPixelTo(uint8_t *source, uint8_t swidth, uint8_t sx, uint8_t sy,
		     uint8_t tx, uint8_t ty)
{
  if(tx>=LCD_LINE_LENGTH || ty>=LCD_LINES)
    return;

  uint8_t byte;

  byte=pgm_read_byte(&source[(sy/LCD_PAGE_HEIGHT)*swidth + sx]);  

  if ( byte & (1<<(sy%LCD_PAGE_HEIGHT)))
    LCD_pixelOn(tx,ty);
  else
    LCD_pixelOff(tx,ty);
}

//------------------------------------------------------------------------
//Copy a range of bytes  form sources sx1<->sx2 on page to LCD tx ty.
// Target coordinates are free - not page dependent. This is much slower
// than writing data on pages with driver function LCD_writeByte(uint8_t)!
//
void LCD_copyPageTo (uint8_t* source, uint8_t swidth, uint8_t spage,
		     uint8_t sx1, uint8_t sx2,uint8_t tx1, uint8_t ty)
{
  uint8_t byte,i,tx2;  
  if ((sx2+tx1)>LCD_LINE_LENGTH)
    tx2=LCD_LINE_LENGTH-1;
  else
    tx2=tx1+(sx2-sx1);    

  for(i=0;i<swidth;i++){
    byte=pgm_read_byte(&source[spage*swidth+i]);  
    if(tx1+i<LCD_LINE_LENGTH && ty < LCD_LINES)
      LCD_writeByteXY(byte,i+tx1,ty);
  }
  
}

//------------------------------------------------------------------------------
//Write a free byte (may be off page) at x,y
//
void LCD_writeByteXY(uint8_t data, uint8_t x, uint8_t y)
{
  uint8_t i;  
  for(i=0;i<8;i++){
    if( data & (1<<i) ) 
      LCD_pixelOn(x,y+i);
    else
      LCD_pixelOff(x,y+i);    
  }
}

//-----------------------------------------------------------------------
//put a graphical glyph on screen defined by font created with  
// created with FontCreator written by F. Maximilian Thiele
//  http://www.apetech.de/fontCreator
//
void LCD_putgchar(uint8_t c, uint8_t* font)
{
  uint8_t i,j,page,data;
  uint8_t x0=LCD_currentX;
  uint8_t y0=LCD_currentY;
  uint16_t index=0;
  uint8_t width=0;

  //see font definition .h file for data struct
  uint8_t height=pgm_read_byte(font+3);
  uint8_t bytes=(height+7)/8;
  uint8_t firstChar=pgm_read_byte(font+4); 
  uint8_t charCount=pgm_read_byte(font+5);

  if (c<firstChar || c>firstChar+charCount) return;
  
  c-=firstChar;

  for(i=0;i<c;i++)
    index+=pgm_read_byte(font+6+i); //6==font width table

  index=index*bytes+charCount+6;
  width=pgm_read_byte(font+6+c);

  for(i=0;i<bytes;i++){
    page=i*width;
    for(j=0;j<width;j++){
      data=pgm_read_byte(font+index+page+j);
      if(height<(i+1)*8)
	data >>= (i+1)*8-height;
      LCD_writeByte(data);
    }
    LCD_writeByte(0x00);
    LCD_setCursorXY(LCD_currentX-width-1,LCD_currentY+8);    
  }
  LCD_setCursorXY(x0+width+1,y0);

  
}
//-----------------------------------------------------------------------------
// put a string at x,y with graphical font (having width and height defined)
//
void LCD_putgstr(uint8_t* string, uint8_t* font, uint8_t x, uint8_t y)
{
  uint8_t i=0;
  LCD_currentX=x;
  LCD_currentY=y;
  while(string[i] != '\0'){
    if(string[i] == '\n'){
      LCD_currentX=0;
      LCD_currentY+=pgm_read_byte(font+3); //font height
    }
    else 
      LCD_putgchar(string[i],font);
    
    i++;
  }
}

