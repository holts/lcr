
/*
// NOTE:  Graphic LCD TEXT Routines

	Screen Layout:
	Ymax
	^ --------------------------|
	|			    |
	|			    |
	|	  LCD Screen	    |
	|			    |
	|			    |
	0 --------------------------> Xmax
	
	Font Data Sequence:
	MSB----->MSB
	||		  ||
	||		  ||
	\/		  \/
	LSB----->LSB
		MSB----->MSB
		||		  ||
		||		  ||
		\/		  \/
		LSB----->LSB
*/

#include "cms015.h"

// LCD Max value for x and y
#define LCD_XMAX	220
#define LCD_YMAX	176

// Total Pages(Vertical Addressin')
#define LCD_PMAX	(LCD_YMAX/8)	

// Selectable Font Types
#define ASC0508		1
#define ASC2448		2

// Defintions of Font Size
#define TXT0508		1
#define TXT2448		2

#ifdef ASC0508
	#include "Asc0508v.h"
#endif
#ifdef ASC2448
	#include "Asc2448v.h"
#endif

unsigned char code ColorDef[16][2]={
{0x00,0x00},			//0-ºÚ
{0xf8,0x00},			//1-ºì
{0x07,0xe0},			//2-ÂÌ
{0x00,0x1f},			//3-À¶
{0xff,0xe0},			//4-rg»Æ
{0x07,0xff},			//5-gb
{0xf8,0x1f},			//6-rb×Ï
{0x7b,0xef},			//7-»Ò
{0x39,0xe7},			//8-Ç³»Ò
{0x78,0x00},			//9-ºì
{0x03,0xe0},			//10-ÂÌ
{0x00,0x0f},			//11-À¶
{0x7b,0xe0},			//12-rg»Æ
{0x03,0xef},			//13-gb
{0x78,0x0f},			//14-rb×Ï
{0xff,0xff}			//15-»Ò
};

#define	RED	ColorDef[0]
#define	GREEN	ColorDef[1]
#define	BLUE	ColorDef[2]
#define	WHITE	ColorDef[3]
#define BLACK	ColorDef[4]
#define	Purple	ColorDef[5]
#define	YELLOW	ColorDef[6]
#define	Sea	ColorDef[7]

// The structure of color data
typedef struct ColorInfo
{
	unsigned char dataH;
	unsigned char dataL;	
}tColor;

// System color definition
tColor fgColor=WHITE;		// Foreground color
tColor bgColor=BLACK;		// Background color

unsigned char TxtRows;	// Rows the LCD can display some font
unsigned char TxtCols;	// Columns the LCD can display some font

unsigned FontSize = TXT0508;
unsigned code 	  *ptrAscFont = 0;

// Global Variables
unsigned char Pnow = 0;		// Current Page Address(Attention for V/H)
unsigned int Ynow = 0;		// Current point's Y position
unsigned int Xnow = 0;		// Current point's X position(Column Address)


// Set the font type for display(eg.TXT0508)
void SetFont(unsigned char fontx)
{
	FontSize = fontx;
	
	if (fontx == TXT0508)
	{
	#ifdef ASC0508
		ptrAscFont = Asc0508;
		TxtRows = LCD_YMAX/8;
		TxtCols = LCD_XMAX/6;
	#endif
	}
	else
	{
	#ifdef ASC2448
		ptrAscFont = Asc2448;
		TxtRows = LCD_YMAX/48;
		TxtCols = LCD_XMAX/24;
	#endif
	}
}

// Put an ASCII char onto the screen
void putchar(unsigned char AscChar)
{
	unsigned char idata i, tmp;
	unsigned char idata Dmax;	// Total Bytes for a char's FontData
	unsigned char idata FontData[16];
	unsigned int idata BaseAddr;	// Base Address for FontData
	
	if (FontSize == TXT0508)
	{
		Dmax = 5;
		FontData[5] = 0x00;		// Insert a SPACE column
	}
	else
	{ Dmax = 16; }
	
	BaseAddr = (AscChar - 0x20)*Dmax;
	
	// Get font data
	for (i = 0; i < Dmax; i++)
	{ FontData[i] = ptrAscFont[BaseAddr + i]; }
	
	// Send font data
	if (Dmax == 5) tmp = 6; else tmp = 8;
	Dmax = Xnow;	// Dmax for temp.var
	for (i = 0; i < tmp; i++)
	{ WriteDAT8(FontData[i]); }
	
	if (FontSize == TXT2448)
	{
		LCD_SetXY(Pnow - 1, Dmax);
		for (i = 8; i < 16; i++)
		{ WriteDAT8(FontData[i]); }
		LCD_SetXY(Pnow + 1, Xnow);
	}
}

// Display xNumber AscChars
void putchars(unsigned char AscChar, unsigned char xNumber)
{
	while(xNumber-- != 0)
	putchar(AscChar);
}

// Tile screen with one letter
void aTile(unsigned char AscChar)
{
	unsigned i;
	
	// Display "ABC"
	for (i = 0; i < TxtRows; i++)
	{
		LCD_SetXY(LCD_PMAX - 1 - i*FontSize, 0);
		putchars(AscChar, TxtCols);
	}
}
