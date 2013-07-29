#ifndef _LCD_PG12864F_H
#define _LCD_PG12864F_H

//作者：nicksean@163.com
//QQ: 4427560
//声明：本代码仅限于个人学习，不得用于商业用途

//T6963C 控制器  8080时序
//此LCD为横屏, 即一个字节对应水平8个点, 高位在左侧.  B7, B6, ... B1, B0
//水平128点, 垂直64点
//
// 1    Frame Ground(0V)
// 2    VSS(0V)
// 3    VDD(5V)
// 4    V0(input) ????
// 5    nWR       | 写控制信号，0为写允许
// 6    nRD       | 读控制信号，0为读允许
// 7    nCS       | 选选信号, 低电平选通
// 8    C/nD      | 指令/数据选择信号, 低电平为数据
// 9    nReset    | 复位信号
//10    D0        | D0~D7数据总线
// ~
//17    D7
//18    FS        | 字体选择


#define _SCREEN_X_DOTS          (128)
#define _SCREEN_X_CHARS         (16)
#define _SCREEN_Y_DOTS          (64)
//#define _SCREEN_BLOCK_WIDTH     (64)
#define _SCREEN_PAGE_NUMBER     (8)	      


#define _GUI_MODE_CVR           0
#define _GUI_MODE_OR            1
#define _GUI_MODE_AND           2
#define _GUI_MODE_FLP           3


void LCD_Point(BYTE Px, BYTE Py, BYTE mode);
void LCD_Line(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode);
void LCD_HLine(BYTE x0, BYTE y0, BYTE x1, BYTE mode);
void LCD_VLine(BYTE x0, BYTE y0, BYTE y1, BYTE mode);
void LCD_Rectangle(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode);
void LCD_RectangleFill(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode);
void LCD_ShowArray(const BYTE *pFont, BYTE x0, BYTE y0, BYTE width, BYTE height, BYTE mode);
void LCD_CLS(void);
void LCD_Test(void);
void LCD_Init(void);


// void clear_screen();
// void init_12864();


#endif //_LCD_PG12864F_H