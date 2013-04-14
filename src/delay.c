/**
  ******************************************************************************
  * @file    Project/main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    16_January-2012
  * @brief   Delay.c
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
#include "stm8s.h"
#include "delay.h"


/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables*/
volatile u32 TimingDelay;

/**
  ******************************************************************************
  * @brief Delay before completing the action
  * @param[in] Delay value in milliseconds
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void Delay(u32 nTime)
{
  /* Wait delay elapsed by interrupt update of timingdelay variable*/
  TimingDelay = nTime;
  while (TimingDelay != 0);
}

/**
  ******************************************************************************
  * @brief Decrement initial value at each interrupt
  * @param[in]
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
