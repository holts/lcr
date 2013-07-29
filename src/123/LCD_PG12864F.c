#include "nx_C51.h"
#include "LCD_PG12864F.h"
#include "T6963.h"

// use external memory but to operate the commands and datas
static volatile BYTE xdata lcd_cmd _at_ LCD_ADDRESS_CMD;
static volatile BYTE xdata lcd_data _at_ LCD_ADDRESS_DAT;




//////////////////////////////////////////////////////////////////////////
//竖屏,x为点, y为字符高
//void LCD_MoveCursorToChar(BYTE x, BYTE yCh)

//横屏,x为字符下标, y为点高度
void LCD_MoveCursorToChar(BYTE xCh, BYTE y)
{
    WORD add;
    
    add = y * _SCREEN_X_CHARS + xCh;
    
    //设置地址
    T6963_WR(_L_OF_WORD(add));
    T6963_WR(_H_OF_WORD(add));
    T6963_CMD(_CMD_REG | _CMD_REG_ADDRESS);    
}


//////////////////////////////////////////////////////////////////////////
//  绘点子程序
//////////////////////////////////////////////////////////////////////////
void LCD_Point(BYTE Px, BYTE Py, BYTE mode) 
{
    BYTE tempD;
    
    if(Px >= _SCREEN_X_DOTS || Py >= _SCREEN_Y_DOTS)
    {
        return;
    }
        
    LCD_MoveCursorToChar(Px >> 3, Py);

    if(mode == _GUI_MODE_CVR || mode == _GUI_MODE_OR)
    {
        T6963_CMD(_CMD_BIT_SET(7 - (Px & 0x07)));
    }
    else if(mode == _GUI_MODE_FLP)
    {
        tempD = T6963_ReadData(No_move);
        _FLP_BIT(tempD, 7 - (Px & 0x07));
        T6963_WriteData(tempD, No_move);
    }
}



//////////////////////////////////////////////////////////////////////////
// 功能：以覆盖的方式画水平线。
// 入口参数：x0		水平线起点所在列的位置
//           y0		水平线起点所在行的位置
//           x1          水平线终点所在列的位置
//           color	显示颜色(对于黑白色LCM，为0时灭，为1时显示)
// 出口参数：无
// 说明：操作失败原因是指定地址超出缓冲区范围。
//////////////////////////////////////////////////////////////////////////
void LCD_HLineCVR(BYTE x0, BYTE x1, BYTE ra0, BYTE ra1)
{
    BYTE dataA, i;

    if(x0 == x1)                                //起止点在同一个块内
    {
        i = T6963_ReadData(No_move);        //仅此一块数据
      
        dataA = (0xFF >> ra0) & (0xFF << 8 - ra1);//数据块的掩码
        i |= dataA;          
            
        T6963_WriteData(i, Increase);        
    }
    else
    {
        i = T6963_ReadData(No_move);        //第一块数据    
        dataA = i | (0xFF >> ra0);          //第一块的前半部分从屏中读出
        T6963_WriteData(dataA, Increase);
            
        for(i = x0 + 1; i < x1; ++i)
        {
            T6963_WriteData(0xFF, Increase);
        }

        i = T6963_ReadData(No_move);        //最后一块数据                
        dataA = i | ~(0xFF >> ra1);      
        T6963_WriteData(dataA, Increase);
    }
}

//////////////////////////////////////////////////////////////////////////
// 功能：画水平线。
// 入口参数：x0		水平线起点所在列的位置
//           y0		水平线起点所在行的位置
//           x1          水平线终点所在列的位置
//           color	显示颜色(对于黑白色LCM，为0时灭，为1时显示)
// 出口参数：无
// 说明：操作失败原因是指定地址超出缓冲区范围。
//////////////////////////////////////////////////////////////////////////
void LCD_HLine(BYTE x0, BYTE y0, BYTE x1, BYTE mode)
{
    BYTE ra0, ra1;
    
    if(x0 > x1) 				// 对x0、x1大小进行排列，以便画图
    {
        _SWAP(x0, x1);
    }

    x1++;                       //转换为半闭区间
    
    ra0 = x0 & 0x07;
    ra1 = x1 & 0x07;
    x0 >>= 3;
    x1 >>= 3;
    
    LCD_MoveCursorToChar(x0, y0);
    
    if(mode == _GUI_MODE_CVR || mode == _GUI_MODE_OR)
    {
        LCD_HLineCVR(x0, x1, ra0, ra1);
    }
    else
    {
    }    
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void LCD_VLine(BYTE x0, BYTE y0, BYTE y1, BYTE mode)
{
    if(y0 > y1)                             // 对y0、y1大小进行排列，以便画图
    {
        _SWAP(y0, y1);
    }
    
    y1++;                                   //转换为半闭区间
    
    for(; y0 < y1; ++y0)
    {
        LCD_Point(x0, y0, mode);	                // 逐点显示，描出垂直线
    }
}


//////////////////////////////////////////////////////////////////////////
void LCD_Rectangle(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode)
{
    LCD_HLine(x0, y0, x1, mode);
    LCD_VLine(x1, y0, y1, mode);
    LCD_VLine(x0, y0, y1, mode);
    LCD_HLine(x0, y1, x1, mode);
}


//////////////////////////////////////////////////////////////////////////
// 名称：LCD_RectangleFill()
// 功能：填充矩形。画一个填充的矩形，填充色与边框色一样。
// 入口参数：x0		矩形左上角的x坐标值
//           y0		矩形左上角的y坐标值
//           x1          矩形右下角的x坐标值
//           y1          矩形右下角的y坐标值
//           color	填充字符
// 出口参数：无
// 说明：操作失败原因是指定地址超出有效范围。
//////////////////////////////////////////////////////////////////////////
void LCD_RectangleFill(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode)
{
    BYTE i;

    if(x0 > x1)
    {
        _SWAP(x1, x0);
    }
    if(y0 > y1)
    {
        _SWAP(y1, y0);
    }
    
    if(x0 == x1 || y0 == y1 || x0 >= _SCREEN_X_DOTS || y0 >= _SCREEN_Y_DOTS)
    {
        return;
    }

    if(x1 > _SCREEN_X_DOTS)
    {
        x1 = _SCREEN_X_DOTS;
    }
    
    if(y1 > _SCREEN_Y_DOTS)
    {
        y1 = _SCREEN_Y_DOTS;
    }
        
    //横屏
    for(i = y0; i <= y1; ++i)
    {
        LCD_HLine(x0, i, x1, mode);
    }

    return;
}




//以连续字节填充方式写入一个指定的连续字节缓存中
//pFont: 要填充的字节数据指针
//bytes: 待填充字模的字节数,不够一字节的按一字节算, 并不是整个字模的大小, 仅是当前行或列的大小
//pos0 : 要填充的起始位置(在屏幕上的以字节为单位的位置, 横屏为行, 竖屏为列)
//pos1 : 要填充的结束位置
//ra0  : 起始点在所在字节的偏移量(向右), 例如:| _ _ _ x x x x x | 偏移量为3
//ra1  : 结束点在所在字节的偏移量(向左), 例如:| x x x x x _ _ _ | 偏移量为5 
void LCD_ShowStreamCVR(const BYTE *pFont, BYTE bytes, BYTE pos0, BYTE pos1, BYTE ra0, BYTE ra1)
{
    BYTE i;
    BYTE dataA, dataB, stuff;
    
    if(pos1 == pos0)                            //起止点在同一个块内
    {
        stuff = T6963_ReadData(No_move);        //仅此一块数据
      
        dataB = (~(0xFF >> ra0)) | (0xFF >> ra1);//数据块的掩码
        dataB &= stuff;                         //去掉屏上待填充部分的数据
        dataA = (*pFont >> ra0) | dataB;        //字模数据填到数据中
            
        T6963_WriteData(dataA, Increase);        
    }
    else
    {
        if(ra0 == 0)
        {
            if(ra1 == 0)
            {
                for(i = pos0; i < pos1; ++i)
                {
                    T6963_WriteData(*pFont++, Increase);
                }
            }
            else
            {
                for(i = pos0; i < pos1; ++i)
                {
                    T6963_WriteData(*pFont++, Increase);
                }
                
                stuff = T6963_ReadData(No_move);//最后一块数据
        
                dataA = stuff & (0xFF >> ra1);
                dataA |= *pFont;
                T6963_WriteData(dataA, Increase);
            }
        }
        else
        {
            stuff = T6963_ReadData(No_move);        //第一块数据
    
            dataB = stuff & (~(0xFF >> ra0));       //第一块的前半部分从屏中读出
            dataA = (*pFont >> ra0) | dataB;        //字模数据前半部分与上次的后半部分合并
            dataB = (*pFont++) << (8 - ra0);        //字模数据后半部分(这里假设了字模空白区填充0)
        
            T6963_WriteData(dataA, Increase);
            
            for(i = pos0 + 1; i < pos1; ++i)
            {
                //移动字模
                dataA = (*pFont >> ra0) | dataB;        //字模数据前半部分与上次的后半部分合并
                dataB = (*pFont++) << (8 - ra0);        //字模数据后半部分(这里假设了字模空白区填充0)

                T6963_WriteData(dataA, Increase);
            }

            if(ra1 != 0)
            {
                stuff = T6963_ReadData(No_move);        //最后一块数据
                
                if(bytes <= 1 || ra0 == ra1)            //当宽度为整字节时, ra0 == ra1
                {
                    dataA = stuff & (0xFF >> ra1) | dataB;
                }
                else
                {
                    dataA = (*pFont >> ra0) | dataB;
                    dataB = stuff & (0xFF >> ra1);
                    dataA = dataB | dataA;
                }
        
                T6963_WriteData(dataA, Increase);
            }
        }
    }
}

//以连续字节填充方式写入一个指定的连续字节缓存中, 与原数据进行"异或"运算
//pFont: 要填充的字节数据指针
//bytes: 待填充字模的字节数,不够一字节的按一字节算, 并不是整个字模的大小, 仅是当前行或列的大小
//pos0 : 要填充的起始位置(在屏幕上的以字节为单位的位置, 横屏为行, 竖屏为列)
//pos1 : 要填充的结束位置
//ra0  : 起始点在所在字节的偏移量(向右), 例如:| _ _ _ x x x x x | 偏移量为3
//ra1  : 结束点在所在字节的偏移量(向右), 例如:| x x x x x _ _ _ | 偏移量为5 
void ShowBMPOneStreamXOR(const BYTE *pFont, BYTE bytes, BYTE pos0, BYTE pos1, BYTE ra0, BYTE ra1)
{
    BYTE i;
    BYTE dataA, dataB, stuff;
    
    if(pos1 == pos0)                            //起止点在同一个块内
    {
        stuff = T6963_ReadData(No_move);        //仅此一块数据
        dataA = (*pFont >> ra0) ^ stuff;        //字模数据与原数据运算
        T6963_WriteData(dataA, Increase);
    }
    else
    {
        if(ra0 == 0)
        {
            if(ra1 == 0)
            {
                for(i = pos0; i < pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff ^ *pFont++, Increase);
                }
            }
            else
            {
                for(i = pos0; i < pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff ^ *pFont++, Increase);
                }
                
                stuff = T6963_ReadData(No_move);//最后一块数据
                T6963_WriteData(stuff ^ *pFont, Increase);
            }
        }
        else
        {
            stuff = T6963_ReadData(No_move);        //第一块数据
    
            dataA = *pFont >> ra0;                  //字模数据前半部分与上次的后半部分合并
            dataB = (*pFont++) << (8 - ra0);        //字模数据后半部分(这里假设了字模空白区填充0)
        
            T6963_WriteData(stuff ^ dataA, Increase);
            
            for(i = pos0 + 1; i < pos1; ++i)
            {
                stuff = T6963_ReadData(No_move);
                
                //移动字模
                dataA = *pFont >> ra0 | dataB;      //字模数据前半部分与上次的后半部分合并
                dataB = (*pFont++) << (8 - ra0);    //字模数据后半部分(这里假设了字模空白区填充0)

                T6963_WriteData(stuff ^ dataA, Increase);
            }

            if(ra1 != 0)
            {
                stuff = T6963_ReadData(No_move);        //最后一块数据
                
                if(bytes <= 1 || ra0 == ra1)            //当宽度为整字节时, ra0 == ra1
                {
                    dataA = dataB;
                }
                else
                {
                    dataA = (*pFont >> ra0) | dataB;
                }
                
                T6963_WriteData(stuff ^ dataA, Increase);
            }
        }
    }
}

//以连续字节填充方式写入一个指定的连续字节缓存中, 与原数据进行"与"运算
//pFont: 要填充的字节数据指针
//pos0 : 要填充的起始位置(在屏幕上的以字节为单位的位置, 横屏为行, 竖屏为列)
//pos1 : 要填充的结束位置
//ra0  : 起始点在所在字节的偏移量(向右), 例如:| _ _ _ x x x x x | 偏移量为3
//ra1  : 结束点在所在字节的偏移量(向右), 例如:| x x x x x _ _ _ | 偏移量为5 
void ShowBMPOneStreamAND(const BYTE *pFont, BYTE bytes, BYTE pos0, BYTE pos1, BYTE ra0, BYTE ra1)
{
    BYTE i;
    BYTE dataA, dataB, stuff;
    
    if(pos1 == pos0)                            //起止点在同一个块内
    {
        stuff = T6963_ReadData(No_move);        //仅此一块数据
        dataA = (*pFont >> ra0) | (~(0xFF >> ra0));  //字模数据与原数据运算
                                                //注意与运算之前要把无用数据填为1
        T6963_WriteData(stuff & dataA, Increase);
    }
    else
    {
        if(ra0 == 0)
        {
            if(ra1 == 0)
            {
                for(i = pos0; i <= pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff & *pFont++, Increase);
                }
            }
            else
            {
                for(i = pos0; i < pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff & *pFont++, Increase);
                }
                
                stuff = T6963_ReadData(No_move);//最后一块数据
                T6963_WriteData(stuff & *pFont, Increase);
            }
        }
        else
        {
            stuff = T6963_ReadData(No_move);        //第一块数据
    
            dataA = (*pFont >> ra0) | (~(0xFF >> ra0));
            dataB = (*pFont++) << (8 - ra0);        //字模数据后半部分(这里假设了字模空白区填充0)
        
            T6963_WriteData(stuff & dataA, Increase);
            
            for(i = pos0 + 1; i < pos1; ++i)
            {
                stuff = T6963_ReadData(No_move);
                
                //移动字模
                dataA = *pFont >> ra0 | dataB;      //字模数据前半部分与上次的后半部分合并
                dataB = (*pFont++) << (8 - ra0);    //字模数据后半部分(这里假设了字模空白区填充0)

                T6963_WriteData(stuff & dataA, Increase);
            }

            stuff = T6963_ReadData(No_move);        //最后一块数据

            if(ra1 == 0)
            {
                dataA = (*pFont >> ra0) | dataB;    //字模数据前半部分与上次的后半部分合并
                T6963_WriteData(stuff & dataA, Increase);
            }                
            else
            {
                dataB |= 0xFF >> ra0;
                T6963_WriteData(stuff & dataB, Increase);
            }
        }
    }
}

//以连续字节填充方式写入一个指定的连续字节缓存中, 与原数据进行"或"运算
//pFont: 要填充的字节数据指针
//pos0 : 要填充的起始位置(在屏幕上的以字节为单位的位置, 横屏为行, 竖屏为列)
//pos1 : 要填充的结束位置
//ra0  : 起始点在所在字节的偏移量(向右), 例如:| _ _ _ x x x x x | 偏移量为3
//ra1  : 结束点在所在字节的偏移量(向右), 例如:| x x x x x _ _ _ | 偏移量为5 
void ShowBMPOneStreamOR(const BYTE *pFont, BYTE bytes, BYTE pos0, BYTE pos1, BYTE ra0, BYTE ra1)
{
    BYTE i;
    BYTE dataA, dataB, stuff;
    
    if(pos1 == pos0)                            //起止点在同一个块内
    {
        stuff = T6963_ReadData(No_move);        //仅此一块数据
        dataA = (*pFont >> ra0) | stuff;        //字模数据与原数据运算
        T6963_WriteData(dataA, Increase);
    }
    else
    {
        if(ra0 == 0)
        {
            if(ra1 == 0)
            {
                for(i = pos0; i <= pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff | *pFont++, Increase);
                }
            }
            else
            {
                for(i = pos0; i < pos1; ++i)
                {
                    stuff = T6963_ReadData(No_move); 
                    T6963_WriteData(stuff | *pFont++, Increase);
                }
                
                stuff = T6963_ReadData(No_move);//最后一块数据
                T6963_WriteData(stuff | *pFont, Increase);
            }
        }
        else
        {
            stuff = T6963_ReadData(No_move);        //第一块数据
    
            dataA = *pFont >> ra0;                  //字模数据前半部分与上次的后半部分合并
            dataB = (*pFont++) << (8 - ra0);        //字模数据后半部分(这里假设了字模空白区填充0)
        
            T6963_WriteData(stuff | dataA, Increase);
            
            for(i = pos0 + 1; i < pos1; ++i)
            {
                stuff = T6963_ReadData(No_move);
                
                //移动字模
                dataA = *pFont >> ra0 | dataB;      //字模数据前半部分与上次的后半部分合并
                dataB = (*pFont++) << (8 - ra0);    //字模数据后半部分(这里假设了字模空白区填充0)

                T6963_WriteData(stuff | dataA, Increase);
            }

            stuff = T6963_ReadData(No_move);        //最后一块数据

            if(ra1 == 0)
            {
                dataA = (*pFont >> ra0) | dataB;    //字模数据前半部分与上次的后半部分合并
                T6963_WriteData(stuff | dataA, Increase);
            }                
            else
            {
                T6963_WriteData(stuff | dataB, Increase);
            }
        }
    }
}




//////////////////////////////////////////////////////////////////////////
//在指定位置处(x0, y0)开始, 画指定大小的(width, height)的图片(pFont),
//可以指定模式(mode: 或, 异或, 与)
void LCD_ShowArray(const BYTE *pFont, BYTE x0, BYTE y0, BYTE width, BYTE height, BYTE mode)
{
    BYTE i;
    BYTE y1, x1, ra0, ra1, fntAdd;
    WORD temp;
    
    if(pFont == NULL || width == 0 || height == 0 || x0 >= _SCREEN_X_DOTS || y0 >= _SCREEN_Y_DOTS)
    {
        return;
    }

    temp = (WORD)x0 + (WORD)width;
    if(temp > _SCREEN_X_DOTS)
    {
        temp = _SCREEN_X_DOTS;
    }
    x1 = (BYTE)temp;
    
    temp = (WORD)y0 + (WORD)height;
    if(temp > _SCREEN_Y_DOTS)
    {
        temp = _SCREEN_Y_DOTS;
    }
    y1 = (BYTE)temp;               //图形填充后右下角在屏上的座标
    
    //竖屏
    //ra0 = y0 & 0x07;            //起始点在字节中的偏移, (除以8后的余数)
    //ra1 = y1 & 0x07;            //终止点在字节中的偏移
    
    //横屏
    fntAdd = (width >> 3) + ((width & 7) > 0);//将宽度单位转换为字节

    ra0 = x0 & 0x07;            //起始点在字节中的偏移, (除以8后的余数)
    ra1 = x1 & 0x07;            //终止点在字节中的偏移

    x0 >>= 3;                   //将点转换为存储单元座标(右移3位即除以8)
    x1 >>= 3;
    
    if(mode == _GUI_MODE_CVR)
    {
        for(i = y0; i < y1; ++i)     //逐行处理
        {
            LCD_MoveCursorToChar(x0, i);
            LCD_ShowStreamCVR(pFont, fntAdd, x0, x1, ra0, ra1);
            pFont += fntAdd;                 
        }
    }
    else if(mode == _GUI_MODE_AND)
    {
        for(i = y0; i < y1; ++i)     //逐行处理
        {
            LCD_MoveCursorToChar(x0, i);
            ShowBMPOneStreamAND(pFont, fntAdd, x0, x1, ra0, ra1);
            pFont += fntAdd;                   
        }
    }
    else if(mode == _GUI_MODE_OR)
    {
        for(i = y0; i < y1; ++i)     //逐行处理
        {
            LCD_MoveCursorToChar(x0, i);
            ShowBMPOneStreamOR(pFont, fntAdd, x0, x1, ra0, ra1);
            pFont += fntAdd;                   
        }
    }
    else if(mode == _GUI_MODE_FLP)
    {
        for(i = y0; i < y1; ++i)     //逐行处理
        {
            LCD_MoveCursorToChar(x0, i);
            ShowBMPOneStreamXOR(pFont, fntAdd, x0, x1, ra0, ra1);
            pFont += fntAdd;                   
        }
    }

    return;
}

//////////////////////////////////////////////////////////////////////////
//画线。任意方向的斜线,直线数学方程 aX+bY=1 
//////////////////////////////////////////////////////////////////////////
// 参数类型有待修改
void LCD_Line(BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE mode)
{
    BYTE t;
    SWORD xerr, yerr, delta_x, delta_y, distance;
    SBYTE incx, incy;
    
    if(x0 == x1)
    {
        LCD_VLine(x0, y0, y1, mode);
        return;        
    } 
    
    if(y0 == y1)
    {
        LCD_HLine(x0, y0, x1, mode);
        return;
    }

    delta_x = x1 - x0;                 //计算坐标增量
    delta_y = y1 - y0;
    
    if(delta_x > 0)
    {
        incx = 1;                       //设置单步方向
    }
    else 
    {
        incx = -1;
        delta_x = -delta_x;
    }
    
    if(delta_y > 0)
    {
        incy = 1;
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    
    distance = (delta_x > delta_y)? delta_x:delta_y;//选取基本增量坐标轴

    xerr = 0, yerr = 0;
    for(t = 0; t <= distance + 1; t++)
    {                                         
        LCD_Point(x0, y0, mode);              //画点
        
        xerr += delta_x;
        yerr += delta_y;
        
        if(xerr > distance)
        {
            xerr -= distance;
            x0 += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            y0 += incy;
        }
    }
}


///----------------------------------------------------------------------------
void LCD_CLS(void)
{
    BYTE i, j, k = 1;
    
    //设置地址
    T6963_WR(0x00);
    T6963_WR(0x00);
    T6963_CMD(_CMD_REG | _CMD_REG_ADDRESS);
    
    //开始自动写入
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_WRITE);

    for(j = 0; j < _SCREEN_Y_DOTS; j++)
    {
        for(i = 0; i < _SCREEN_X_CHARS; i++)
        {
            T6963_AutoWR(0x00);
        }
    }
    
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_RESET);
}

//test
void LCD_Test(void)
{
    /*
    BYTE i, j, k;
    //设置地址
    T6963_WR(0x00);
    T6963_WR(0x00);
    T6963_CMD(_CMD_REG | _CMD_REG_ADDRESS);
    
    T6963_WR(0xF0);
    //开始自动写入
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_WRITE);

    for(j = 0; j < _SCREEN_Y_DOTS; j++)
    {
        for(i = 0; i < _SCREEN_X_CHARS; i++)
        {
            T6963_AutoWR(i);
        }
    }
    
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_RESET);*/
}

//清空RAM内容(比清屏幕还彻底)
void LCD_ClearRAM(void)
{
    WORD i;
    
    //设置地址
    T6963_WR(0x00);
    T6963_WR(0x00);
    T6963_CMD(_CMD_REG | _CMD_REG_ADDRESS);
    
    //开始自动写入
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_WRITE);

    for(i = 0x0000; i < 0x2000; i++)            //8K 全部清空
    {
        T6963_AutoWR(0x00);
    }
    
    T6963_CMD(_CMD_AUTO | _CMD_AUTO_RESET);
}


//初始化
void LCD_Init(void)
{
    T6963_InitBus();
    
    LCD_RST_EN;
    Delay1(200);
    LCD_RST_UN;

    //图形起始地址
    T6963_WR(0x00);
    T6963_WR(0x00);
    T6963_CMD(_CMD_CTRL | _CMD_CTRL_GRAPHIC_HOME);

    //图形区域
    T6963_WR(_SCREEN_X_CHARS);   //240个点, 除以8, 即30个字符宽
    T6963_WR(0x00);
    T6963_CMD(_CMD_CTRL | _CMD_CTRL_GRAPHIC_AREA);
    
    //光标形状
    T6963_CMD(_CMD_CURSOR | _CMD_CURSOR_8);
    
    //显示方式
    T6963_CMD(_CMD_MODE | _CMD_MODE_OR | _CMD_MODE_CG_ROM);
    
    LCD_ClearRAM();
    
    //显示开关
    T6963_CMD(_CMD_DISPLAY | _CMD_DISPLAY_GRAPHIC_ON);
}


/*
void delay(BYTE t)
{
    WORD data i;
    WORD data j;
    for(i = 0; i < t; ++i)
    {
        for(j = 0; j < 114; ++j);
    }
}


void write_data(BYTE dat)
{
    while((lcd_cmd & 0x03) != 0x03);
    lcd_data = dat;
}

void write_cmd1(BYTE cmd)
{
    while((lcd_cmd & 0x03) != 0x03);
    lcd_cmd = cmd;
}

void write_cmd2(BYTE dat, BYTE cmd)
{
    while((lcd_cmd & 0x03) != 0x03);
    lcd_data = dat;
    while((lcd_cmd & 0x03) != 0x03);
    lcd_cmd = cmd;
}


void write_cmd3(BYTE dat1, BYTE dat2, BYTE cmd)
{
    while((lcd_cmd & 0x03) != 0x03);
    lcd_data = dat1;
    while((lcd_cmd & 0x03) != 0x03);
    lcd_data = dat2;
    while((lcd_cmd & 0x03) != 0x03);
    lcd_cmd = cmd;
}


void clear_screen()
{
    WORD i;
    write_cmd3(0x00, 0x00, 0x24);   //地址指针设置
    write_cmd1(0xB0);   //数据自动写，地址自加一
    for(i = 0; i < 1024; ++i)
    {
        write_data(0xFF);
    }
    write_cmd1(0xB2);   //数据自动写结束
}

void init_12864()
{
    RST_EN;
    delay(10);
    RST_UN;

    write_cmd3(0x01, 0x00, 0x21);   //光标指针设置
    write_cmd3(0x00, 0x00, 0x40);   //文本区首地址
    write_cmd1(0x80);   //显示方式设置，正常显示
    write_cmd1(0x94);   //显示状态设置 1 0 1 0 N3 N2 N1 N0 文本显示，光标不显示，不闪烁
    write_cmd1(0x98);   //图形方式显示，不显示字母，只打点
    write_cmd1(0xa1);   //光标形状设置 1 0 1 0 0 N2 N1 N0
}
*/