/**
  ******************************************************************************
  * @file    HD44780.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    16_January-2012
  * @brief   HD44780.c
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

/* Includes ------------------------------------------------------------------*/
#include "HD44780.h"
#include "main.h"
#include "delay.h"
#include "stdarg.h"
#include "stdio.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#if defined(_RAISONANCE_)
/* Do not remove for Raisonance compiler */
void dummy(void)
{
  /*printf declaration enables vsprintf definition*/
  printf("");
}
#endif


/**
  * @addtogroup HD44780_Functions
  * @{
  */


/**
  * @brief  Initializes Character Generator CGRAM with custom characters data
  * @param  Table containing characters definition values
  * @param  Number of characters defined in the table
  * @retval None
  */
void LCD_LOAD_CGRAM(char tab[], u8 charnum)
{
  u8 index;
  /* Each character contains 8 definition values*/
  charnum = charnum * 8;
  for (index = 0;index < charnum;index++)
  {
    /* Store values in LCD*/
    LCD_printchar(tab[index]);
    Delay(1);
  }
}


/**
  * @brief  Activate Power Pin that supplies LCD module
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_PWRON (void)
{
  GPIO_WriteHigh(LCDPwrPort, LCDPwrPin);
}

/**
  * @brief  Disable Power Pin that supplies LCD module
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_PWROFF (void)
{
  GPIO_WriteLow(LCDPwrPort, LCDPwrPin);
}

/**
  * @brief  Activate Enable Pin from LCD module
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_ENABLE (void)
{
  GPIO_WriteHigh(LCDControlPort, LCD_Enable);
  Delay(2);
  GPIO_WriteLow(LCDControlPort, LCD_Enable);
}

/**
  * @brief  Command data sent to LCD module
  * @param  command value to be sent
  * @param  None
  * @retval None
  */
void LCD_CMD(unsigned char cmd_data)
{
  GPIO_WriteLow(LCDControlPort, LCD_RS);
  GPIO_Write(LCDPort, (GPIO_ReadOutputData(LCDPort) & 0xF0) | ((cmd_data >> 4) & 0x0F));
  LCD_ENABLE();
  GPIO_Write(LCDPort, (GPIO_ReadOutputData(LCDPort) & 0xF0) | (cmd_data & 0x0F));
  LCD_ENABLE();
  Delay(2);
}


/**
  * @brief  Initializes HD44780 LCD module in 4-bit mode
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_INIT(void)
{
  GPIO_WriteHigh(LCDControlPort, LCD_Enable);
  GPIO_WriteLow(LCDControlPort, LCD_RS);
  //Initialization of HD44780-based LCD (4-bit HW)
  LCD_CMD(0x33);
  Delay(4);
  LCD_CMD(0x32);
  Delay(4);
  //Function Set 4-bit mode
  LCD_CMD(0x28);
  //Display On/Off Control
  LCD_CMD(0x0C);
  //Entry mode set
  LCD_CMD(0x06);
  LCD_CLEAR_DISPLAY();
  //Minimum delay to wait before driving LCD module
  Delay(200);
}

/**
  * @brief  Print Character on LCD module
  * @param  Ascii value of character
  * @param  None
  * @retval None
  */
void LCD_printchar(unsigned char ascode)
{
  GPIO_WriteHigh(LCDControlPort, LCD_RS);
  GPIO_Write(LCDPort, (GPIO_ReadOutputData(LCDPort) & 0xF0) | ((ascode >> 4) & 0x0F));
  LCD_ENABLE();
  GPIO_Write(LCDPort, (GPIO_ReadOutputData(LCDPort) & 0xF0) | (ascode & 0x0F));
  LCD_ENABLE();
  Delay(2);
}

/**
  * @brief  Display of a characters string
  * @param  Text to be displayed
  * @param  None
  * @retval None
  */
void LCD_printstring(unsigned char *text)
{
  do
  {
    LCD_printchar(*text++);
  }
  while (*text != '\n');
}

/**
  * @brief  Clear LCD module display
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_CLEAR_DISPLAY(void)
{
  LCD_CMD(0x01);
  Delay(2);
}

/**
  * @brief  Set Cursor on second row 1st digit
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_2ndROW(void)
{
  LCD_CMD(0xC0);
}

/**
  * @brief  Set Cursor to Home position
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_HOME(void)
{
  LCD_CMD(0x02);
  Delay(2);
}

/**
  * @brief  Shift display to left
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_LSHIFT(void)
{
  LCD_CMD(0x18);
}

/**
  * @brief  Shift display to right
  * @param  Text to be displayed
  * @param  None
  * @retval None
  */
void LCD_RSHIFT(void)
{
  LCD_CMD(0x1C);
}

/**
  * @brief  Set Display On
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_DISP_ON(void)
{
  LCD_CMD(0x0C);
}

/**
  * @brief  Set Display Off
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_DISP_OFF(void)
{
  LCD_CMD(0x08);
}

/**
  * @brief  Set Cursor to a specified location given by row and column information
  * @param  Row Number (1 to 2)
  * @param  Column Number (1 to 16) Assuming a 2 X 16 characters display
  * @retval None
  */
void LCD_LOCATE(u8 row, u8 column)
{
  column--;
  switch (row)
  {
    case 1:
      /* Set cursor to 1st row address and add index*/
      LCD_CMD(column |= 0x80);
      break;
    case 2:
      /* Set cursor to 2nd row address and add index*/
      LCD_CMD(column |= 0x40 | 0x80);
      break;
    default:
      break;
  }
}

/**
  * @brief  lcd printf function
  * @param  string with standard defined formats
  * @param
  * @retval None
  */
void LCD_printf(const char *fmt, ...)
{
  s32 i;
  u32 text_size, letter;
  static char text_buffer[32];
  va_list args;

  va_start(args, fmt);
  text_size = vsprintf(text_buffer, fmt, args);

  // Process the string
  for (i = 0; i < text_size; i++)
  {
    letter = text_buffer[i];

    if (letter == 10)
      break;
    else
    {
      if ((letter > 0x1F) && (letter < 0x80))
        LCD_printchar(letter);
    }
  }
}



/**
  * @}
  */

/**
* @}
*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
