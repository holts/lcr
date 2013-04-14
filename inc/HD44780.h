/**
  ********************************************************************************
  * @file    HD44780.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    16-January-2012
  * @brief   This file contains all functions prototypes and macros for the HD44780 LCD controller.
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
#ifndef __HD44780_H
#define __HD44780_H


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* LCD HW settings */
/* LCDPort contains 4-bit data D0 to D3*/
/* LCDPort must be assigned to the chosen port */
//#define LCDPort GPIOB
/* PD0,PD1,PD2,PD3 */
#define LCDPort GPIOD

/* Pins E and RS of LCD must be assigned to LCDControlPort*/
//#define LCDControlPort GPIOC
#define LCDControlPort GPIOG
/* LCD Enable pin is assigned to Px1 */
//#define LCD_Enable GPIO_PIN_1
#define LCD_Enable GPIO_PIN_0
/* LCD RS pin is assigned to Px2 */
//#define LCD_RS GPIO_PIN_2
#define LCD_RS GPIO_PIN_1
 
/* Define port where LCD Power is connected */
#define LCDPwrPort GPIOE
/* LCD Power Supply pin is assigned to Px5 */
#define LCDPwrPin GPIO_PIN_5

/* HD44780 CGRAM address start */
#define CGRAM_address_start 0x40

/* Exported constants --------------------------------------------------------*/
/* Exported macros ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @addtogroup HD44780_Exported_Functions
  * @{
  */
void LCD_LOAD_CGRAM(char tab[], u8 charnum);
void LCD_PWRON (void);
void LCD_PWROFF (void);
void LCD_INIT(void);
void LCD_printchar(unsigned char ascode);
void LCD_printstring(unsigned char *text);
void LCD_CMD(unsigned char data);
void LCD_CLEAR_DISPLAY(void);
void LCD_2ndROW(void);
void LCD_HOME(void);
void LCD_LSHIFT(void);
void LCD_RSHIFT(void);
void LCD_DISP_ON(void);
void LCD_DISP_OFF(void);
void LCD_LOCATE(u8 row, u8 column);
void LCD_printf(const char *fmt, ...);


#endif /* __HD44780_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
