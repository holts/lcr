#ifndef   CONFIG_H 
#define   CONFIG_H 

/* LCD HW settings ----------------------------------------------------------*/ 
//#define LCD_8BIT
//#define LCD_FUNCTION	0x38	// LCD mode as 8-bit 2-line 5*8-dot 1/16Duty
#define LCD_4BIT
#define LCD_FUNCTION	0x28	// LCD mode as 4-bit 2-line 5*8-dot 1/16Duty

/* LCDPort contains 4-bit data D0 to D3*/
/* LCDPort must be assigned to the chosen port */
#define LCDPort P0

/* Pins E and RS of LCD must be assigned to LCDControlPort*/
#define LCDControlPort P0

/* LCD Enable pin is assigned to Px1 */
#define LCD_Enable_Pin    4
/* LCD R/W pin is assigned to Px5 */
#define LCD_RW_Pin        5
/* LCD RS pin is assigned to Px2 */
#define LCD_RS_Pin        6
/* Exported constants --------------------------------------------------------*/
#define iDat        	1
#define iCmd		0

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
#define LCD_ROW 	3
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

/* Exported macros -----------------------------------------------------------*/
#define SET_RS		sbi(LCDPort, LCD_RS_Pin)
#define SET_RW 		sbi(LCDPort, LCD_RW_Pin)
#define SET_EN 		sbi(LCDPort, LCD_Enable_Pin)
#define CLR_RS 		cbi(LCDPort, LCD_RS_Pin)
#define CLR_RW 		cbi(LCDPort, LCD_RW_Pin)
#define CLR_EN 		cbi(LCDPort, LCD_Enable_Pin)

#endif
