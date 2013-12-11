
#ifndef LCD_GRAPHICS
#define LCD_GRAPHICS

#define     int8_t       char
#define     uint8_t      unsigned  char
#define     uint16_t     unsigned  short int
#define     uint32_t     unsigned  int


//Graphics functions
void LCD_drawLine (uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1);
void LCD_clearLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1);
void LCD_invertLine(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1);

void LCD_drawCirc  (uint8_t x1, uint8_t y1, uint8_t radius);
void LCD_fillCirc  (uint8_t x1, uint8_t y1, uint8_t radius);
void LCD_clearCirc (uint8_t x1, uint8_t y1, uint8_t radius);
void LCD_invertCirc(uint8_t x1, uint8_t y1, uint8_t radius);

void LCD_drawFullBMP(uint8_t* bitmap);
void LCD_drawBMP(uint8_t* bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void LCD_putgchar(uint8_t c, uint8_t* font);
void LCD_putgstr(uint8_t* string, uint8_t* font, uint8_t x, uint8_t y);

void LCD_writeByteXY(uint8_t data, uint8_t x, uint8_t y);
void LCD_copyPixelTo(uint8_t* source, uint8_t swidth, uint8_t sx, uint8_t sy, uint8_t tx, uint8_t ty);
void LCD_copyPageTo (uint8_t* source, uint8_t swidth, uint8_t spage,
		     uint8_t sx1, uint8_t sx2,uint8_t tx1, uint8_t ty);

void LCD_drawRect  (uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void LCD_clearRect (uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void LCD_fillRect  (uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);

//helper functions:
uint8_t _LCD_absDiff(uint8_t a,uint8_t b);
void _LCD_doRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,
		 void (*pPixFun)(uint8_t, uint8_t ),
		 void (*pPageFun)(uint8_t, uint8_t, uint8_t ) );
void _LCD_doLine (uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,
		  void (*pPixFun)(uint8_t, uint8_t ) );
void _LCD_doCirc(uint8_t x1, uint8_t y1, uint8_t radius,
		 void (*pLineFun)(uint8_t,uint8_t,uint8_t,uint8_t) );


#endif //LCD_GRAPHICS
