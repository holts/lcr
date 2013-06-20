
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HD44780_H
#define HD44780_H

void LCD_Load_CGRAM(char tab[], unsigned char charnum);
bit LCD_Check_Busy(void);
void LCD_INIT(void);
void LCD_PrintChar(unsigned char ascode);
void LCD_PrintString(unsigned char *text);
void LCD_CMD(unsigned char cmd_data);
void LCD_Clear(void);
void LCD_2ndRow(void);
void LCD_Home(void);
void LCD_LShift(void);
void LCD_RShift(void);
void LCD_CursorOn(void);
void LCD_CursorOff(void);
void LCD_Locate(unsigned char row, unsigned char column);

/*
#define LCD_CTRL_DDR	DDRA
#define LCDPort		PORTB
#define LCD_DI		PINB
#define LCD_DATA_DDR	DDRB
*/

#endif

