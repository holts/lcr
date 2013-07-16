
#ifndef   CMS015_H 
#define   CMS015_H 

void LCD_INIT(void);
void WriteDAT8(unsigned char b);     //写8位数据
void WriteCOM(u8 b);
void WriteDAT(u8 a,u8 b); //写16位数据
void LCD_CMD(u16 cmd,u16 dat);
//void LCD_SetWin(u8 x1,u8 y1,u8 x2,u8 y2);	//设置显示窗口
void LCD_SetXY(u8 x,u8 y);
//void LCD_ReSetWin(void);	//恢复原始窗口

#endif
