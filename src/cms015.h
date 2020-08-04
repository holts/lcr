#ifndef __CMS015_H__
#define __CMS015_H__
/* { */

#include <stdint.h>

/* GPIOx config 
 *
 * PB6  LCD_CS
 * PB5  LCD_RS
 * PB4  LCD_WR
 * PB3  LCD_RD
 * PB1  LCD_RST
 * PB8 - PB15  LCD_D8-D15
 *
 * */
#define LCD_GPIO    GPIOB
#define LCD_RESET_PIN   GPIO_Pin_1
#define LCD_RD_PIN      GPIO_Pin_3
#define LCD_WR_PIN      GPIO_Pin_4
#define LCD_RS_PIN      GPIO_Pin_5
#define LCD_CS_PIN      GPIO_Pin_6

#define	RST_1    GPIO_SetBits(LCD_GPIO, LCD_RESET_PIN)   // 高电平
#define	RST_0    GPIO_ResetBits(LCD_GPIO, LCD_RESET_PIN) // 低电平  
#define	RS_1    GPIO_SetBits(LCD_GPIO, LCD_RS_PIN)   //  高电平
#define	RS_0    GPIO_ResetBits(LCD_GPIO, LCD_RS_PIN) //  低电平  
#define	WR_1    GPIO_SetBits(LCD_GPIO, LCD_WR_PIN)   //  高电平
#define	WR_0    GPIO_ResetBits(LCD_GPIO, LCD_WR_PIN) //  低电平  
#define	RD_1    GPIO_SetBits(LCD_GPIO, LCD_RD_PIN)   //  高电平
#define	RD_0    GPIO_ResetBits(LCD_GPIO, LCD_RD_PIN) //  低电平  
#define	CS_1    GPIO_SetBits(LCD_GPIO, LCD_CS_PIN)   //  高电平
#define	CS_0    GPIO_ResetBits(LCD_GPIO, LCD_CS_PIN) //  低电平  

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
//extern void lcd_write (lcd_cd_t cd, uint8_t byte);
extern void WriteCMD (uint8_t cd);
extern void WriteDAT (uint8_t b);
extern void lcd_write_cmd (uint16_t cmd, uint16_t dat);
extern void lcd_write_data (uint8_t a, uint8_t b);
extern void lcd_clear (void);
extern void lcd_gotoxy (uint8_t x ,uint8_t y);
extern void lcd_putchar (const char c);
extern void lcd_putstr (const char *str,int fill);
extern void lcd_putnum (int x, int y,char *str);
void lcd_setcontrast(int c);

extern char mask;

#endif
