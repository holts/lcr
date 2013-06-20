
#include <reg52.h>
#include <intrins.h>
#include "macros.h"
#include "delay.h"
#include "config.h"
#include "ptask.h"

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
//#define LCD_L3		0x90
//#define LCD_L4		0xD0

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
#define SET_RS		sbi(LCDPort, LCD_RS_Pin)
#define SET_RW 		sbi(LCDPort, LCD_RW_Pin)
#define SET_EN 		sbi(LCDPort, LCD_Enable_Pin)
#define CLR_RS 		cbi(LCDPort, LCD_RS_Pin)
#define CLR_RW 		cbi(LCDPort, LCD_RW_Pin)
#define CLR_EN 		cbi(LCDPort, LCD_Enable_Pin)


/**
  * @brief  Check busy
  * @param  None
  * @param  None
  * @retval if busy return true
  */
bit LCD_Check_Busy(void) 
{ 
  LCDControlPort = 0xFF; 
  CLR_RS; 
  SET_RW; 
  CLR_EN; 
  _nop_(); 
  SET_EN;
  return (bit)(LCDControlPort & 0x80);
}

/**
  * @brief  Command data sent to LCD module
  * @param  command value to be sent
  * @param  None
  * @retval None
  */
unsigned char LCD_CMD(unsigned char cmd_data)
{  
  while(LCD_Check_Busy()); //忙则等待
  CLR_RS; 
  CLR_RW; 
  SET_EN; 
  LCDControlPort = cmd_data; 
  _nop_(); 
  CLR_EN;
}

/**
  * @brief  Set Cursor on second row 1st digit
  * @param  None
  * @param  None
  * @retval None
  */
unsigned char LCD_2ndRow(void)
{ LCD_CMD(LCD_L2); }

/**
  * @brief  Set Cursor to Home position
  * @param  None
  * @param  None
  * @retval None
  */
unsigned char LCD_Home(void)
{
  LCD_CMD(LCD_HOME);
  _SS
	  while(1) 
	   {WaitX(2);}
  _EE
  //DelayMs(2);
}

/**
  * @brief  Shift cursor to left
  * @param  None
  * @param  None
  * @retval None
  */
unsigned char LCD_LShift(void)
{ LCD_CMD(0x18); }

/**
  * @brief  Shift cursor to right
  * @param  None
  * @param  None
  * @retval None
  */
unsigned char LCD_RShift(void)
{ LCD_CMD(0x1C); }

/**
  * @brief  Set Display cursor
  * @param  None
  * @param  None
  * @retval None
  */
unsigned char LCD_CursorOn(void)
{ LCD_CMD(0x0C); }

/**
  * @brief  Set Hide cursor
  * @param  None
  * @param  None
  * @retval None
  */
unsigned char LCD_CursorOff(void)
{ LCD_CMD(0x08); }

/**
  * @brief  Set Cursor to a specified location given by row and column information
  * @param  Row Number (1 to 2)
  * @param  Column Number (1 to 16) Assuming a 2 X 16 characters display
  * @retval None
  */
unsigned char LCD_Locate(unsigned char row, unsigned char column)
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
#if defined(LCD_L3)
  else if (row == 3)
  {
  }
#endif
#if defined(LCD_L4)
  else if (row == 4)
  {
  }
#endif
  
}


/**
  * @brief  Print Character on LCD module
  * @param  Ascii value of character
  * @param  None
  * @retval None
  */
unsigned char LCD_PrintChar(unsigned char ascode)
{ 
  while(LCD_Check_Busy()); //忙则等待
  SET_RS; 
  CLR_RW; 
  SET_EN; 
#ifdef LCD_4BIT
  LCDPort = ascode & 0xF0;  //Higher 4-bit
  _nop_();
  CLR_EN;
  _nop_();
  SET_EN;
  LCDPort = ascode << 4;    //Lower 4-bit
#elif defined(LCD_8BIT)
  LCDPort = ascode; 
#endif
  _nop_();
  CLR_EN;
}

/**
  * @brief  Display of a characters string
  * @param  Text to be displayed
  * @param  None
  * @retval None
  */
unsigned char LCD_PrintString(unsigned char *text)
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
unsigned char LCD_Load_CGRAM(char tab[], unsigned char charnum)
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
  * @param  None
  * @retval None
  */
unsigned char LCD_Clear(void)
{
  LCD_CMD(LCD_CLS);
  DelayMs(2);
}

/**
  * @brief  Initializes HD44780 LCD module in 4-bit mode
  * @param  None
  * @param  None
  * @retval None
  */
unsigned char LCD_INIT(void)
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


/*------------------------------------------------
// PORT initialization
void port_ini(void)
{
	LCD_CTRL_DDR = 0x07;
	LCD_DATA_DDR = 0xFF;
}

// Write a Data or Command to LCD
void SendByte(u8 DatCmd, u8 dByte)
{
	if (DatCmd == 0)
		CLR_RS;
	else
		SET_RS;
	CLR_RW;
	PTK_BEGIN
		while(1)
		{
	#ifdef LCD_4BIT
		SET_EN;
		LCDPort = dByte & 0xF0;	//Higher 4-bit
		//DelayUs(50);
		Task_delay(1);
		CLR_EN;
		//DelayUs(50);
		Task_delay(1);
		SET_EN;
		LCDPort = dByte << 4;	//Lower 4-bit
		//DelayUs(50);
		Task_delay(1);
		CLR_EN;
	#elif defined(LCD_8BIT)
		SET_EN;
		LCDPort = dByte;
		//DelayUs(50);
		Task_delay(1);
		CLR_EN;
	#endif
		}
	PTK_END
}

// Write a string to LCD
void SendStr(u8 *ptString)
{
	while((*ptString)!='\0')
	{ SendByte(iDat, *ptString++); }
}

// Send a Number less than 255
void SendNbr(u8 Number)
{
	u8 *temp;
	temp = NumberToChar(Number);
	SendByte(iDat, *temp++);
	SendByte(iDat, *temp++);
	SendByte(iDat, *temp);
}

// Move Cursor or display
void Move(u8 dir)
{ SendByte(iCmd, dir); }

// Goto specific location
void Gotoxy(u8 Row, u8 Col)
{
	switch (Row)
	{
		#if defined(LCD_L2)
		case 2:
			SendByte(iCmd, LCD_L2 + Col); break;
		#endif
		#if defined(LCD_L3)
		case 3:
			SendByte(iCmd, LCD_L3 + Col); break;
		#endif
		#if defined(LCD_L4)
		case 4:
			SendByte(iCmd, LCD_L4 + Col); break;
		#endif
		default:
			SendByte(iCmd, LCD_L1 + Col); break;
	}	
}

// Clear LCD Screen
void Clear(void)
{
	SendByte(iCmd, LCD_CLS);
	//DelayMs(3);
	Task_delay(2);
	// 2ms delay is Necessary after sending LCD_CLS command !!!
}

// Wait some time and clear screen
void wait_and_clear(void)
{
	//DelayMs(1500);
	Task_delay(1500);
	Clear();
}

// Fill CGRAM with array CGRAM[]
void FillCGRAM(void)
{
	u8 i;
	SendByte(iCmd, LCD_CGRAM_ADDR);
	for (i = 0; i < LCD_CGMAX; i++)
	{ SendByte(iDat, CGRAM[i]); }
}

// Show All patterns in CGRAM
void ShowCGRAM(void)
{
	u8 i,k;
	for (i = 0; i < 8; i++)
	{
		SendByte(iCmd, LCD_L1);
		for (k = 0; k < LCD_CHAR; k++)
		{
			#if defined(LCD_L2)
			switch (k)
			{
				case LCD_COL:
				SendByte(iCmd, LCD_L2); break;
				#if defined(LCD_L3)
				case LCD_COL*2:
				SendByte(iCmd, LCD_L3); break;
				#endif
				#if defined(LCD_L4)
				case LCD_COL*3:
				SendByte(iCmd, LCD_L4); break;
				#endif
				default:
				break;
			}
			#endif
			SendByte(iDat, i);
		}
		wait_and_clear();
	}
}

// Call built-in Charactors
void CallBultinChar(void)
{
	u8 i, k;
	for (i = 0; i < LCD_COL; i += LCD_ROW)
	{
		SendByte(iCmd, LCD_L1);
		for (k = 0; k < LCD_CHAR; k++)
		{
			#if defined(LCD_L2)
			switch (k)
			{
				case LCD_COL:
				SendByte(iCmd, LCD_L2); break;
				#if defined(LCD_L3)
				case LCD_COL*2:
				SendByte(iCmd, LCD_L3); break;
				#endif
				#if defined(LCD_L4)
				case LCD_COL*3:
				SendByte(iCmd, LCD_L4); break;
				#endif
				default:
				break;
			}
			#endif
			SendByte(iDat, k + LCD_COL*i);
		}
		wait_and_clear();
	}
}
// LCD initialize procedure
void LCD_Initial(void)
{
	delay100ms();				// Wait for internal initialization
	port_ini();
	#ifdef LCD_4BIT
		CLR_RS;
		CLR_RW;
		SET_EN;
		LCD_DO = 0x20;	// Set Interface to 4-bit
		DelayUs(50);
		CLR_EN;
	#endif
	SendByte(iCmd, LCD_FUNCTION);	// Function Set
	DelayUs(50);
	SendByte(iCmd, LCD_ON);
	DelayUs(50);
	Clear();
	SendByte(iCmd, LCD_ENTRY);	// Entry Mode Set
	*/
	/*
	// Display system Info
	#ifdef LCD_4BIT
		Gotoxy(1, 4);
		SendStr("LCD_4BIT");
	#else
		Gotoxy(1, 4);
		SendStr("LCD_8BIT");
	#endif	
	Gotoxy(2,1);
	SendStr("LCD_COLS: ");
	SendNbr(LCD_COL);
	Gotoxy(3,1);
	SendStr("LCD_ROWS: ");
	SendNbr(LCD_ROW);
	Gotoxy(4,1);
	SendStr("LCD_CHAR: ");
	SendNbr(LCD_CHAR);
	wait_and_clear();
	*/
	/*
}

*/
