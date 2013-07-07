
#include <reg52.h>
#include <intrins.h>
#include "macros.h"
#include "delay.h"

/***********************************************************************
*MCU stc12c5a60s2
*晶振 32MHz
*LCD1602 4 位数据线程序
*   端口名称              端口符号    LCD引脚　  MCU端口   
*   RS数据/指令选择线        RS         4         P0^6     0命令1数据
*   R/W读写选择线            R/W        5         P0^5     0写1读
*   EN写使能线               EN         6         P0^4     使能位,下降沿触发
*   数据总线                 D0-D3     7-10       NC
*                            D4         11        P0^3
*                            D5         12        P0^2
*                            D6         13        P0^1
*                            D7         14        P0^0
*   其他端口连接：
*   电源负                   VSS        1      
*   电源正+4.5--+5.5V        VDD        2
*   对比度调节 (接地最大)    VL         3 
*   背光负                   BLK VSS    15    
*   背光正                   BLA VDD    16
***********************************************************************/

#define LCD_1602
//#define LCD_2004

//#define LCD_8BIT      0x38	// LCD mode as 8-bit 2-line 5*8-dot 1/16Duty
#define LCD_4BIT	0x28	// LCD mode as 4-bit 2-line 5*8-dot 1/16Duty

/* LCDPort contains 4-bit data D4 to D7 */
#define LCDPort P0

/* Pins E,R/W and RS of LCD must be assigned to LCDControlPort*/
#define LCDControlPort P0

/* LCD Enable pin is assigned to Px1 */
#define LCD_EN          4
/* LCD R/W pin is assigned to Px5 */
#define LCD_RW          5
/* LCD RS pin is assigned to Px2 */
#define LCD_RS          6

/* Exported constants ------------------------------------------------*/

#define LCD_CLS		0x01		// Clear LCD screen
#define LCD_HOME	0x02		// LCD Return home
#define LCD_ENTRY 	0x06		// Set LCD Entry Mode
#define LCD_C2L		0x10		// Move Cursor to the left
#define LCD_C2R		0x14		// Move Cursor to the right
#define LCD_D2L		0x18		// Move display to the left
#define LCD_D2R		0x1C		// Move display to the right

#define LCD_ON		0x0C		// Turn on LCD and Cursor
#define LCD_OFF		0x08		// Turn off LCD

#define LCD_CGRAM_ADDR	0x40		// Start address of LCD CGRAM
#define LCD_CGMAX 	64		// Max CGRAM bytes

#define LCD_COL 	16
#define LCD_ROW 	2
#define LCD_CHAR	LCD_COL*LCD_ROW

#define LCD_L1		0x80
#define LCD_L2		0xC0
#define LCD_L3		0x90
#define LCD_L4		0xD0

const unsigned char CGRAM[LCD_CGMAX] =
{
	// Make eight patterns of 8*5 font
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	// 1.Dsiplay All
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// 2.White Board
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,	// 3.Virtical 1
	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,	// 4.Virtical 2
	0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,	// 5.Horizon 1
	0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,	// 6.Horizon 2	
	0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,	// 7.Stars
	0xFF,0x11,0x11,0x11,0x11,0x11,0x11,0xFF,	// 8.Rectangle
};

/* Exported macros ---------------------------------------------------*/
#define SET_RS		sbi(LCDControlPort, LCD_RS)
#define SET_RW 		sbi(LCDControlPort, LCD_RW)
#define SET_EN 		sbi(LCDControlPort, LCD_EN)
#define CLR_RS 		cbi(LCDControlPort, LCD_RS)
#define CLR_RW 		cbi(LCDControlPort, LCD_RW)
#define CLR_EN 		cbi(LCDControlPort, LCD_EN)


/**
  * @brief  Check busy
  * @param  None
  * @retval if busy return true
  */
bit LCD_Check_Busy(void) 
{ 
#ifdef LCD_8BIT
  LCDPort = 0xFF; 
#elif defined(LCD_4BIT)
  LCDPort = 0xF0;
#endif
  CLR_RS; 
  SET_RW; 
  CLR_EN; 
  _nop_(); 
  SET_EN;
  return (bit)(LCDPort & 0x80);
}

/**
  * @brief  Command data sent to LCD module
  * @param  command value to be sent
  * @retval None
  */
void LCD_CMD(unsigned char cmd_data)
{  
  while(LCD_Check_Busy()); //忙则等待
  CLR_RS; 
  CLR_RW; 

#ifdef LCD_4BIT
  SET_EN; 
  LCDPort = ( (LCDPort & 0x0F) | ((cmd_data << 4) & 0xF0) ); 
  _nop_(); 
  CLR_EN;
  _nop_();
  SET_EN;
  LCDPort = ( (LCDPort & 0x0F) | (cmd_data & 0xF0) );
  _nop_();
  CLR_EN;
#elif  defined(LCD_8BIT)
  SET_EN; 
  LCDPort = cmd_data; 
  _nop_(); 
  CLR_EN;
#endif  
}

/**
  * @brief  Set Cursor on second row 1st digit
  * @param  None
  * @retval None
  */
void LCD_2ndRow(void)
{ LCD_CMD(LCD_L2); }

/**
  * @brief  Set Cursor to Home position
  * @param  None
  * @retval None
  */
void LCD_Home(void)
{
  LCD_CMD(LCD_HOME);
  DelayMs(2);
}

/**
  * @brief  Shift cursor to left
  * @param  None
  * @retval None
  */
void LCD_LShift(void)
{ LCD_CMD(0x18); }

/**
  * @brief  Shift cursor to right
  * @param  None
  * @retval None
  */
void LCD_RShift(void)
{ LCD_CMD(0x1C); }

/**
  * @brief  Set Display cursor
  * @param  None
  * @retval None
  */
void LCD_CursorOn(void)
{ LCD_CMD(0x0C); }

/**
  * @brief  Set Hide cursor
  * @param  None
  * @retval None
  */
void LCD_CursorOff(void)
{ LCD_CMD(0x08); }

/**
  * @brief  Set Cursor to a specified location given by row and column information
  * @param  Row Number (1 to 2)
  * @param  Column Number (1 to 16) Assuming a 2 X 16 characters display
  * @retval None
  */
void LCD_Locate(unsigned char row, unsigned char column)
{
  column--;
  if (row == 1)
  {   /* Set cursor to 1st row address and add index*/
      LCD_CMD(column |= LCD_L1);
  } 
  else if (row == 2)
  {   /* Set cursor to 2nd row address and add index*/
      LCD_CMD(column |= LCD_L2);
  }
#if defined(LCD_2004)
  else if (row == 3)
  {   LCD_CMD(column |= LCD_L3); }
  else if (row == 4)
  {   LCD_CMD(column |= LCD_L4); }
#endif
  
}


/**
  * @brief  Print Character on LCD module
  * @param  Ascii value of character
  * @retval None
  */
void LCD_PrintChar(unsigned char ascode)
{ 
  while(LCD_Check_Busy()); //忙则等待
  SET_RS; 
  CLR_RW; 	
#ifdef LCD_4BIT
  SET_EN; 
  LCDPort = ( (LCDPort & 0x0F) | ((ascode << 4) & 0xF0) );  //Lower 4-bit
  _nop_();
  CLR_EN;
  _nop_();
 
  SET_EN; 
  LCDPort =  ( (LCDPort & 0x0F) | (ascode & 0xF0) );   //Higher 4-bit
  _nop_();
  CLR_EN;
#elif defined(LCD_8BIT)
  SET_EN; 
  LCDPort = ascode;   
  _nop_();
  CLR_EN;
#endif
}

/**
  * @brief  Display of a characters string
  * @param  Text to be displayed
  * @retval None
  */
void LCD_PrintString(unsigned char *text)
{
 do { LCD_PrintChar(*text++); } while (*text != '\n');
 //while(*text) {LCD_printchar(*text++);}
}

/**
  * @brief  Initializes Character Generator CGRAM with custom characters data
  * @param  Table containing characters definition values
  * @param  Number of characters defined in the table
  * @retval None
  */
void LCD_Load_CGRAM(char tab[], unsigned char charnum)
{
  unsigned char index;
  /* Each character contains 8 definition values*/
  charnum = charnum * 8;
  for (index = 0;index < charnum;index++)
  {
    /* Store values in LCD*/
    LCD_PrintChar(tab[index]);
    DelayMs(1);
  }
}


/**
  * @brief  Clear LCD module display
  * @param  None
  * @retval None
  */
void LCD_Clear(void)
{
  LCD_CMD(LCD_CLS);
  DelayMs(2);
}

/**
  * @brief  Initializes HD44780 LCD module in 4-bit mode
  * @param  None
  * @retval None
  */
void LCD_INIT(void)
{
  SET_EN;
  CLR_RS;
  //Initialization of HD44780-based LCD (4-bit HW)
  LCD_CMD(0x33);
  DelayMs(4);
  LCD_CMD(0x32);
  DelayMs(4);
  //Function Set 4-bit mode
  LCD_CMD(0x28);
  //Cursor On/Off Control
  LCD_CMD(0x0C);
  //Entry mode set
  LCD_CMD(0x06);
  LCD_Clear();
  //Minimum delay to wait before driving LCD module
  DelayMs(200);
}

