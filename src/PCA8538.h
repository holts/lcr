
#ifndef     PCA8538_H
#define     PCA8538_H


void DlyMs(int n);
void PCA8538_SPI_Init(void);
void PCA8538_WriteData(unsigned char XPointer, unsigned char YPointer, unsigned char Data);

void Clear_LCD_Dianzhen(void);
 
void lcd_cls(void); //清屏 
void lcd_init(void);

void LCD_SetXY(unsigned char x,unsigned char y);

void lcd_goto1(unsigned char x); //第1行x位
void lcd_goto2(unsigned char x); //第2行x位
void lcd_goto3(unsigned char x); //第3行x位
void lcd_goto4(unsigned char x); //第4行x位

//取点阵数据
//unsigned char *getASC(unsigned char ASC);
//字符输出
void lcd_putc(uchar d);
//字串输出
void lcd_puts(uchar *s);

#endif
