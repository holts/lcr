

#include "stdafx.h"
#include "..\LCD.h"
#include "..\LCDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#include <util/delay.h>
//#include <avr/io.h>
//#include <avr/pgmspace.h>
#include "lcd_driver_KS0107.h"
#include "lcd_graphics.h"
#include "girl_1_glcd.h"
#include "atmel_glcd.h"

#include "arial_bold_14.h"
//#include "corsiva_12.h"

void wait_100x_ms(int ms) {	::Sleep(ms*100); }

void  _delay_ms(uint8_t i)
{
	::Sleep(1000*i);
}

int  posy = 0,posx = 0;
void lcdTest(void);
void drawTest(int type);

void lcdTest(void)
{
  //LCD_init
  drawTest(7);
  
  while(1) {   
    /* 
    drawTest(0); //simple text    
    drawTest(1); //simple drawing
    drawTest(2); //more simple drawing
    drawTest(3); //scrolling text up-down
    drawTest(4); //menus with invert
    drawTest(5); //reading status of LCD
    drawTest(6); //simple progress bar
    drawTest(7); //128x64 bmp on screen   
    drawTest(8); //put a bmp on screen (atmel picture is 108x35)    
    drawTest(9); 
    */
    drawTest(10);
  }  

}

void drawTest(int type){
  uint8_t i,j;
  
  uint8_t menuStrings[5][8] = {"Menu1\0","Menu2\0","Menu3\0","Menu4\0","Menu5\0"};
  
  switch (type){

  case 0: //simple text on screen
    LCD_clr();
    LCD_setCursorXY(15,0);
    LCD_puts ((uint8_t*)"Hello AVR world!\n\0");
    LCD_putsp((uint8_t*)"This LCD uses KS0107\0",1,3);
    LCD_putsp((uint8_t*)"Too long paged text is wrapped automatically\0",5,3);
    _delay_ms(10);
    break;

  case 1: //some of the drawing functions
    LCD_clr();
    LCD_drawLine(0,0,127,63);
    LCD_drawLine(0,63,127,0);
    LCD_drawRect(10,10,117,53);
    LCD_fillCirc(63,31,15);
    _delay_ms(10);
    break;

  case 2: //write on screen byte by byte
    break;

  case 4:  //using invert for menus
    LCD_clr();
    for(i=0;i<5;i++){ LCD_putsp(menuStrings[i],i+1,25); }

    j=2;
    while(j--){
      LCD_invertPage(1,20,58);
      wait_100x_ms(3);
      for(i=0;i<5;i++){      
	wait_100x_ms(3);
	LCD_invertPage(i+1,20,58);
	if(i<4)
	  LCD_invertPage(i+2,20,58);
	wait_100x_ms(1);
      }
    }
    _delay_ms(1);
    break;

  case 5: //read status (on /off)
    break;

  case 6: //"progress bar"
    LCD_clr();
    LCD_drawRect(5,5,122,58);
    LCD_putsp((uint8_t*)"Prosessing...\0",2,16);
    LCD_drawRect(10,30,113,41);

    for(j=0;j<100;j++){     
      LCD_setCursorXY(12+j,32);
      LCD_writeByte(0xff);
      wait_100x_ms(1); 
    }
    LCD_clr();
    LCD_putsp((uint8_t*)"Done!        \0",2,16);
    _delay_ms(1);
    break;

  case 7:
    LCD_clr();
    LCD_drawFullBMP(girl_1_glcd_bmp);
    _delay_ms(1);
    break;

  case 8:
    i=0xaa;
    LCD_clr();
    LCD_drawBMP((uint8_t*)atmel_glcd_bmp, 12, 12, 
		  ATMEL_GLCD_WIDTH,ATMEL_GLCD_HEIGHT);

    _delay_ms(10);
    break;

  case 9:
    LCD_clr();
    
    //LCD_putgstr((uint8_t*)"Corsiva 12 test", Corsiva_12,10,10);
    //LCD_putgstr((uint8_t*)"qwertyhjkl12345", Corsiva_12,12,30);
    posx ++;
    posy ++;
    posx = posx% 60;
    posy = posy% 60;

    LCD_putgstr((uint8_t*)"putgstr() test",Arial_Bold_14,posx,posy);
    //LCD_putgstr((uint8_t*)"1q2wj3eLiM4t6",Arial_Bold_14,10,30);

    _delay_ms(10);
    break;

  case 10: 
    LCD_clr();
    LCD_setCursorXY(2,2);
    LCD_puts ((uint8_t*)"FUNC:");
    LCD_setCursorXY(32,2);
    LCD_puts ((uint8_t*)"AUTO ");
    LCD_setCursorXY(62,2);
    LCD_puts ((uint8_t*)"RANG:");
    LCD_setCursorXY(92,2);
    LCD_puts ((uint8_t*)"100K ");    
    LCD_setCursorXY(122,2);
    LCD_puts ((uint8_t*)"FREQ:");
    LCD_setCursorXY(152,2);
    LCD_puts ((uint8_t*)"7800Hz");

    LCD_setCursorXY(2,10);
    LCD_puts ((uint8_t*)"GAIN:");
    LCD_setCursorXY(32,10);
    LCD_puts ((uint8_t*)"27   ");
    LCD_setCursorXY(62,10);
    LCD_puts ((uint8_t*)"MODE:");
    LCD_setCursorXY(92,10);
    LCD_puts ((uint8_t*)"P    ");    
    LCD_setCursorXY(122,10);
    LCD_puts ((uint8_t*)"COMP:");
    LCD_setCursorXY(152,10);
    LCD_puts ((uint8_t*)"xxxxK");
    
     _delay_ms(10);
    break;

  }
}
