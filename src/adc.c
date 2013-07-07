
#include <reg52.h>
#include <intrins.h>  
#include "delay.h"

//AD转换
sfr P1ASF = 0x9D;     //将P1置为模拟口寄存器(使能),各位中为1的有效
sfr ADC_CONTR = 0xBC; //A/D转换控制寄存器
sfr ADC_RES   = 0xBD; //A/D转换结果寄存器
sfr ADC_RESL  = 0xBE; //A/D转换结果寄存器

/***********************************************************************
Function : ADC_Init 
Note     : ADC初始化
***********************************************************************/
void ADC_Init(unsigned char Channel){  
	P1ASF = 0x01<<Channel;  //选择ADC的8个接口中的一个（对应位为1选中）  
        ADC_CONTR = 0x40;       //ADC转换的速度140个时钟周期（0XX0 0000 其中XX控制速度）  
        _nop_();  
        ADC_CONTR |= Channel;       //选择A/D当前通道  
        _nop_();  
        ADC_CONTR |= 0x80;      //启动A/D电源  
        DelayMs(1);             //使输入电压达到稳定（1ms即可）  
    }  
      
/***********************************************************************
Function : ADC_Read
Note     : 读ADC值
***********************************************************************/
unsigned int ADC_Read (void){  
        ADC_CONTR |= 0x08;      //启动A/D转换（0000 1000 令ADCS = 1）  
        _nop_();  
        _nop_();  
        _nop_();  
        _nop_();  
	while ( !(ADC_CONTR & 0x10) ); //等待A/D转换结束(ADC_FLAG==0)
        ADC_CONTR &= 0xE7;      //1111 0111 清ADC_FLAG位, 关闭A/D转换,   
   return (ADC_RES*4+ADC_RESL);//返回A/D转换结果（10位ADC数据高8位在ADC_RES中，低2位在ADC_RESL中）  
    }  





 
