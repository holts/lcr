 // STC89C52RC         EEOPROM大小： 4K，首址：0x2000，触发：0x5A，0xA5 
 // STC11F01E          EEOPROM大小： 2K，首址：0x0000，触发：0x5A，0xA5 
 // STC12C5410AD  EEOPROM大小：12K，首址：0x0000，触发：0x46，0xB9 
 // STC12C4052         EEOPROM大小：10K，首址：0x0000，触发：0x46，0xB9
 // STC12C5A32S2       EEOPROM大小：28K，首址：0x0000，触发：0x5A，0xA5
 // STC12C5A60S2       EEOPROM大小： 1K，首址：0x0000，触发：0x5A，0xA5
 // STC15F104E         EEOPROM大小： 1K，首址：0x0000，触发：0x5A，0xA5
 // STC15F204EA        EEOPROM大小： 1K，首址：0x0000，触发：0x5A，0xA5
 // 
 //  连续读、写函数明确指针指向数据类型可以减小代码和提高速度
 //  5V单片机在3.7V以下禁止操作EEPROM，而单片机在3.3V以上程序仍可正常运行。
 //  3.3V单片机在2.4V以下禁止操作EEPROM，而单片机在2.2V以上程序仍可正常运行。
 
#include <reg52.h> 
#include <intrins.h> 
#include "config.h"
 
#define  USE_ISR       1      //程序中是否使用中断（1:是，0:否）
         
#define  USE_EROM_read_continuous     0
#define  USE_EROM_write_continuous    0
#define  USE_EROM_read_word           0
#define  USE_EROM_write_word          0
         
#define  STC12x5Axx_H

//EEOPROM首址（不同STC型号有所不同）
#if defined (__STC10xxx_H__)\
  || defined (__STC11xxx_H__)\ 
  || defined (__STC12xx052_H__)\
  || defined (__STC12x52xx_H__)\
  || defined (__STC12x54xx_H__)\
  || defined (__STC12x56xx_H__)\
  || defined (STC12x5Axx_H)\
  || defined (__STC15F104E_H__)\
  || defined (__STC15F204EA_H__)  
           #define  EROM_START    0x0000 
#elif defined (__STC90x51_H__)
           #define  EROM_START    0x1000
#elif defined (__STC89x51_H__) || defined (__STC89x52_H__) || defined (__STC90x52_H__) 
           #define  EROM_START    0x2000
#elif defined (__STC90x12_H__)
           #define  EROM_START    0x3000
#elif defined (__STC89x54_H__) || defined (__STC90x54_H__) 
           #define  EROM_START    0x4000
#elif defined (__STC89x58_H__) || defined (__STC90x58_H__)
           #define  EROM_START    0x8000
#elif defined (__STC89x510_H__) || defined (__STC90x510_H__)
           #define  EROM_START    0xA000        
#elif defined (__STC89x512_H__) || defined (__STC90x512_H__)
           #define  EROM_START    0xC000
#elif defined (__STC89x514_H__) || defined (__STC90x514_H__)
           #define  EROM_START    0xE000        
#elif
     #warning "STC型号未定义，相应宏定义 EROM_START 缺失"  
#endif
 
 
//ISP触发命令（不同STC型号有所不同）
#if defined (__STC10xxx_H__)\
  || defined (__STC11xxx_H__)\ 
  || defined (__STC12x52xx_H__)\
  || defined (STC12x5Axx_H)\
  || defined (__STC15F104E_H__)\
  || defined (__STC15F204EA_H__) //STC10xxx, STC11xxx, STC12x5Axx, 12x52xx系列ISP触发命令: 0x5A，0xA5      
     #define  ISPTRIG()           ISP_TRIG = 0x5A, ISP_TRIG = 0xA5                
#elif defined (__STC89_90xxx_H__)\ 
  || defined (__STC12xx052_H__)\
  || defined (__STC12x54xx_H__)\
  || defined (__STC12x56xx_H__) //STC89xxx, STC90xxx, STC12xx052, STC12x54xx, STC12x56xx系列ISP触发命令: 0x46，0xB9      
     #define  ISPTRIG()           ISP_TRIG = 0x46, ISP_TRIG = 0xB9
#elif
      #warning "STC型号未定义，相应宏定义 ISPTRIG() 缺失"         
#endif        
 
 
#if   ( CPU_FOSC >= 24000000 )
     #define  EN_ISP  0x80          //系统时钟 >= 24MHz
#elif ( CPU_FOSC >= 20000000 )
     #define  EN_ISP  0x81          //系统时钟 >= 20MHz
#elif ( CPU_FOSC >= 12000000 )
     #define  EN_ISP  0x82          //系统时钟 >= 12MHz
#elif ( CPU_FOSC >=  6000000 )
     #define  EN_ISP  0x83          //系统时钟 >= 6MHz
#elif ( CPU_FOSC >=  3000000 )
     #define  EN_ISP  0x84          //系统时钟 >= 3MHz
#elif ( CPU_FOSC >=  2000000 )
     #define  EN_ISP  0x85          //系统时钟 >= 2MHz
#elif ( CPU_FOSC >=  1000000 )
     #define  EN_ISP  0x86          //系统时钟 >= 1MHz
#else
     #define  EN_ISP  0x87          //系统时钟 < 1MHz
#endif
         
                                 
/********************************************************************
函数：IAP触发命令 + 禁止IAP操作（清IAP相关寄存器）
********************************************************************/
static void command_and_close( void )
 {
 #if (USE_ISR)  
         EA = 0;   
 #endif
 
     ISPTRIG();                
     _nop_(); 
     _nop_(); 
     _nop_();
  
 #if (USE_ISR)
         EA = 1;  
 #endif
           
     ISP_CONTR = 0;    
     ISP_CMD   = 0;          
     ISP_TRIG  = 0;                   
     ISP_ADDRH = 0xFF;      
     ISP_ADDRL = 0xFF;              
 }
 
/********************************************************************
 函数：读指定地址一字节数据                   
 输入：（地址） 
 返回：（地址内容） 
********************************************************************/
unsigned char EROM_read_byte(unsigned int addr)
 {                      
     ISP_CONTR = EN_ISP;   
     ISP_CMD   = 1;                   //允许读取EEPROM
     ISP_ADDRL = addr;                
     ISP_ADDRH = (unsigned char)(addr>>8) + (unsigned char)(EROM_START>>8);               
     command_and_close();
           
     return ISP_DATA;                        
 }
 
/********************************************************************
 函数：向指定地址写一字节数据
 输入：（地址，数据）
********************************************************************/
void EROM_write_byte(unsigned int addr, unsigned char dat)
 {     
     ISP_CONTR = EN_ISP;        
     ISP_CMD   = 2;                  //允许编写EEPROM
     ISP_ADDRL = addr;                
     ISP_ADDRH = (unsigned char)(addr>>8) + (unsigned char)(EROM_START>>8);           
     ISP_DATA  = dat;
                                   
     command_and_close();    
 }
 
/********************************************************************
 函数：擦除指定扇区          
 输入：（扇区号）
********************************************************************/
void EROM_erase_sector(unsigned char n)
 {           
     ISP_CONTR = EN_ISP; 
     ISP_CMD   = 3;            //允许擦除EEPROM     
     ISP_ADDRL = 0x00;                
     ISP_ADDRH = (unsigned char)(EROM_START>>8) + n*0x02; 
                                            
     command_and_close();
 }
 
/********************************************************************
 函数：连续读数据                   
 输入：（起始地址，指针指向，字节长）
********************************************************************/
#if (USE_EROM_read_continuous)
void EROM_read_continuous(unsigned int addr, unsigned char READPointer *pr, unsigned char len)
 {                         
     while (len--)
     { 
         *pr = EROM_read_byte(addr);
         pr++;
         addr++;
     }                                    
 }
#endif
 
/********************************************************************
 函数：连续写数据
 输入：（起始地址，指针指向，字节长）
********************************************************************/
#if (USE_EROM_write_continuous)
void EROM_write_continuous(unsigned int addr, unsigned char WRITEPointer *pr, unsigned char len)
 {
     while (len--)
     { 
         EROM_write_byte(addr, *pr); 
         pr++;
         addr++;
     }                  
 }        
#endif
 
/********************************************************************
 函数：读指定地址一字数据        
********************************************************************/
#if (USE_EROM_read_word)
unsigned int EROM_read_word(unsigned int addr)
 {                      
     unsigned int dat;
   
     ((unsigned char *)&dat)[0] = EROM_read_byte(addr++);
     ((unsigned char *)&dat)[1] = EROM_read_byte(addr);
     return dat;                        
 }
#endif
 
/********************************************************************
 函数：向指定地址写一字数据
********************************************************************/
#if (USE_EROM_write_word)
void EROM_write_word(unsigned int addr, unsigned int dat)
 {     
     EROM_write_byte(addr++, ((unsigned char *)&dat)[0]); 
     EROM_write_byte(addr, ((unsigned char *)&dat)[1]);   
 }
#endif

