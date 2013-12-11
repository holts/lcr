
#ifndef LCD_DRIVER_KS0107
#define LCD_DRIVER_KS0107

//#include <inttypes.h>
//#include <avr/io.h>


#define     int8_t       char
#define     uint8_t      unsigned  char
#define     uint16_t     unsigned  short int
#define     uint32_t     unsigned  int


//LCD spec definitions
#define LCD_PAGE_HEIGHT 8   //8 lines per page
#define LCD_LINES       64
//#define LCD_LINE_LENGTH 128
#define LCD_LINE_LENGTH 220

#define LCD_NUM_CHIPS   2


#define LEFT  0
#define RIGHT 1


//Function declarations for low level operations
void    LCD_sendCommand(uint8_t command);
void    LCD_writeByte(uint8_t data);
uint8_t LCD_readByte(void);
uint8_t LCD_readStatus(void);
void    LCD_selectChip(void);
void    LCD_selectLeftChip(void);
void    LCD_selectRightChip(void);

//API function declarations
//void LCD_init(void);   //do reset and int LCD
//void LCD_on(void);     //turn on  LCD (not backlight)
//void LCD_off(void);    //turn off LCD (not backlight)

void LCD_clr(void);
void LCD_allPagesOn(uint8_t byte);

void LCD_setCursorXY(uint8_t x, uint8_t y);
void LCD_pixelOn(uint8_t x, uint8_t y);
void LCD_pixelOff(uint8_t x, uint8_t y);
void LCD_invertPixel(uint8_t x, uint8_t y);

void LCD_invertPage(uint8_t page, uint8_t x1, uint8_t x2);
void LCD_onPage(uint8_t page,uint8_t x1,uint8_t x2);
void LCD_offPage(uint8_t page,uint8_t x1,uint8_t x2);

void LCD_putchar(uint8_t c);
void LCD_puts(uint8_t* string);
void LCD_putsp(uint8_t* string, uint8_t page, uint8_t x);

//uint8_t LCD_isBusy(void);
//uint8_t LCD_isOff(void);
//uint8_t LCD_isReseting(void);

//Functions to set RAM addresses for read and write operations
//void LCD_setPageAddress(uint8_t page);
//void LCD_setColumnAddress(uint8_t col);
//void LCD_setInitialLineAddress(uint8_t line);

//help functions
//void LCD_wait_execution(void);
//void LCD_wait_busy(void);
//void LCD_selectSide(uint8_t side);
//uint8_t _LCD_readByte(void);
uint8_t  pgm_read_byte( uint8_t *data );


#endif //Define LCD_DRIVER

