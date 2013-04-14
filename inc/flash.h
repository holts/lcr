/**
  ******************************************************************************
  * @file    delay.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    16-January-2012
  * @brief   This file contains all the prototypes/macros for the delay.c
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * FOR MORE INFORMATION PLEASE READ CAREFULLY THE LICENSE AGREEMENT FILE
  * LOCATED IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

#define     EEPMASS1     0xAE                   //ÃÜÂëÔ¿³×1
#define     EEPMASS2     0x56                   //ÃÜÂëÔ¿³×2
#define     EEPADDR      0x4000                 //eepµØÖ·ÆðÊ¼

#define     EEP_PROG_OFF                     FLASH_IAPSR_DUL = 0   //¹Ø±Õeep
#define     EEPROM_WaitForLastOperation()    while(!FLASH_FLAG_EOP)

#define     EEPLEN    66



/* Exported types ------------------------------------------------------------*/
void Write_Byte (unsigned int address,unsigned char Data);

void eep_unlock(void);
void eep_word_read(u8 *data,u8 len,u16 eep_addr);
void eep_word_write(u8 *data,u8 len,u16 eep_addr);

#endif /* __FLASH_H */







/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
