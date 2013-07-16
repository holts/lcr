#ifndef EEPROM_H
#define EEPROM_H
 
#include "config.h"

extern unsigned char EROM_read_byte(unsigned int addr);              //读指定地址一字节数据
extern void  EROM_write_byte(unsigned int addr, unsigned char dat);  //向指定地址写一字节数据
extern void  EROM_erase_sector(unsigned char n);               //擦除指定扇区
extern void  EROM_read_continuous(unsigned int addr, unsigned char READPointer *pr, unsigned char len);   //连续读出EROM内容到指定缓存
extern void  EROM_write_continuous(unsigned int addr, unsigned char WRITEPointer *pr, unsigned char len); //将缓存内容连续写入EROM
extern unsigned int EROM_read_word(unsigned int addr);             //读指定地址一字数据
extern void  EROM_write_word(unsigned int addr, unsigned int dat); //向指定地址写一字数据

#endif



