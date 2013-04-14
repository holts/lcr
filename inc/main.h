 /**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    16-January-2012
  * @brief   This file contains all the prototypes/macros for main.c.
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
void CLK_Configuration(void);
void GPIO_Configuration(void);
void TIM1_Config(void);
void TIM4_Config(void);
void ADC_Config(void);

void KeyRead(void);
u8 Key_thread(void);
 

static void LoadValueIntoSinStruct(void);

 
/* Global variables ----------------------------------------------------------*/
#define TIM4_PERIOD_MS       124

/* On board push button definition */
#define BUTTON_PORT GPIOD
#define BUTTON_PIN GPIO_PIN_7

//定义选择0度或90度座标系
#define CORD_OUT_PORT       GPIOD
#define CORD_0C_PIN         GPIO_PIN_4   
#define CORD_90C_PIN        GPIO_PIN_3
#define CORD_SEL_PORT GPIOD
#define CORD_SEL_PIN  GPIO_PIN_2
#define CORD_SEL_0C   GPIO_WriteLow(CORD_SEL_PORT,CORD_SEL_PIN) 
#define CORD_SEL_90C  GPIO_WriteHigh(CORD_SEL_PORT,CORD_SEL_PIN)


 
#define       nbr_of_samples_half 30
#define       COUNTER             5
#define       INIT_TIM2_ARR       0xCB   //203
#define       INIT_TIM1_CCR1      0x65
#define       AVERAGE_AMP         0x65
 
/* Private typedef -----------------------------------------------------------*/
typedef struct {
  u8 Value[nbr_of_samples_half];
  u8 Index;
  bool Up;
}Sin_Typedef;




#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
