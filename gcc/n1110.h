#ifndef __N1110_H__
#define __N1110_H__
/* { */

#include <stdint.h>

/* GPIOx config */
#define LCD_GPIO		GPIOB
#define LCD_RESET_PIN	GPIO_Pin_6
#define LCD_SCK_PIN		GPIO_Pin_3
#define LCD_SDA_PIN		GPIO_Pin_4
#define LCD_CS_PIN		GPIO_Pin_5

/**************************************
 * Display macro
 **************************************/

/* LCD memory definitions */
#define LCD_XMAX		98
#define LCD_YMAX		9

///* LCD point array definitions */
//#define LCD_POINT_XMAX	96
//#define LCD_POINT_YMAX	68

/**************************************
 * Type definitions
 **************************************/

/* Command or data */
typedef enum
{
	COMMAND = 0,
	DATA = 1
} lcd_cd_t;

/* Font descriptor type */
typedef struct
{
	uint16_t char_width;
	uint16_t char_offset;
} font_descriptor_t;

/* Font info type */
typedef struct
{
	uint16_t font_height;
	unsigned char start_char;
	unsigned char end_char;
	const font_descriptor_t* descr_array;
	const unsigned char* font_bitmap_array;
} font_info_t;

extern void lcd_init (int mode);
extern void lcd_hw_reset (void);
extern void lcd_write (lcd_cd_t cd, uint8_t byte);
extern void lcd_clear (void);
extern void lcd_gotoxy (uint8_t x ,uint8_t y);
extern void lcd_putchar (const char c);
extern void lcd_putstr (const char *str,int fill);
extern void lcd_putnum (int x, int y,char *str);
void lcd_setcontrast(int c);

extern char mask;

#endif
