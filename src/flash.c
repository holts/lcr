

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "flash.h"

void eep_unlock(void)               //开启eep编程
{
   FLASH->CR1 |= 0x01;
   do
   {
    FLASH->DUKR=EEPMASS1;                        //锁1密钥  
    FLASH->DUKR=EEPMASS2;                        //锁2密钥
   }
   while(!(FLASH->IAPSR&0x08));                  //等待解密就绪         
}

/**
  *****************************************************************
  * @brief flash write byte
  * @par Parameters: address
  * @par Parameters: data
  * @retval void None
  * @par Required preconditions: None
  *****************************************************************
  */
void Write_Byte (unsigned int address,unsigned char Data)
{   
    FLASH->DUKR = 0xAE;
    FLASH->DUKR = 0x56;
    FLASH->CR2 = 0x00;
    FLASH->NCR2 = 0xFF;
    *((u8*)address) = Data;
    FLASH->IAPSR = (u8)(~0x08);
}

/*
读取eep
unsigned char *data   将读到的数据放进指针p
unsigned char len  连续读取的数据个数
unsigned short eep_addr 写入开始地址
*/
void eep_word_read(u8 *data,u8 len,u16 eep_addr)
{
      u8 *eep=(u8*)eep_addr;
      u8  j;
      for(j=0; j<len;j++)
      {
          *data=*eep;            
          eep++;
          data++;
      }
}
/*
unsigned char *data  要写入的数据来源
unsigned char len  写入的数据数量，此处必须大于4个字节，否则不能启动编程
unsigned short eep_addr 写入开始地址
*/
void eep_word_write(u8 *data,u8 len,u16 eep_addr)
{
  u8 *eep=(u8*)eep_addr;
  u8  j;
  eep_unlock();
  for(j=0;j<len;j++)
   {
      *eep=*data;
       data++;
       eep++;
       EEPROM_WaitForLastOperation();
    }
   EEP_PROG_OFF;         
}


