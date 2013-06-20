
#include "intrins.h"

//延时X微秒(STC12C5A60S2@12M) 不同的工作环境,需要调整此函数
void DelayXus(unsigned char n)
{
    while (n--)
    {
        _nop_();
        _nop_();
    }
}

// mS延时函数，含有输入参数 unsigned char t，无返回值
// unsigned char 是定义无符号字符变量，其值的范围是
// 0~255 这里使用晶振12M，精确延时请使用汇编
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayXus(245);
	 DelayXus(245);
 }
}
