
#ifndef   CMS015_H 
#define   CMS015_H 

void LCD_INIT(void);
void WriteDAT8(unsigned char b);     //写8位数据
void WriteCMD(unsigned char b);
void WriteDAT(unsigned char a,unsigned char b); //写16位数据
void LCD_CMD(unsigned int cmd,unsigned int dat);
//void LCD_SetWin(u8 x1,u8 y1,u8 x2,u8 y2);	//设置显示窗口
void LCD_SetXY(unsigned char x,unsigned char y);
//void LCD_ReSetWin(void);	//恢复原始窗口

#endif
