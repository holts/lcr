/**
  ******************************************************************************
  * @file     stm8s_it.c
  * @author   MCD Application Team
  * @version  V2.1.0
  * @date     18-November-2011
  * @brief    Main Interrupt Service Routines.
  *           This file provides template for all peripherals interrupt service
  *           routine.
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
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "ptask.h"
#include "delay.h"
#include "parameter.h"
#include "main.h"
 
 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern u8 temp_AD_H;	// temporary registers for reading ADC result
extern u8 temp_AD_L;
extern u8 ADInit;	// flag for the first conversion
extern u8 ADSampRdy;	// flag for filed of samples ready
extern u8 AD_samp;	// counter of raugh samples
extern u16 AD_sample[NUMB_SAMP];	// store field of raugh samples
extern u16 AD_avg_value;

extern  volatile u8 timers[MAXTASKS];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
extern void TimingDelay_Decrement(void);
//extern void Toggle(void);
//extern volatile bool ButtonPressed;
//extern volatile bool timeON;
extern volatile bool event;
extern volatile u16 ten_ms; 
extern volatile u16 sec;
extern volatile u16 min;
 
//extern volatile u8 KeyDelay;
//extern volatile u16 DispDelay;
//extern volatile u16 BeepDelay;

extern Sin_Typedef sin;
extern volatile counter;

volatile u16 ms = 0;



#ifdef _COSMIC_
/**
  * @brief Dummy Interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#ifdef STM8S903
/**
  * @brief External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208 || STM8AF52Ax */

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
	u8 PWM_Value = 0;
		
  if (counter >= COUNTER)               
  { 
    /* Reset the counter value*/
    counter = 0;
    /* Upper half cycle of sine wave */
    if (sin.Up == TRUE)
    {
      /* Load the duty cycle register for upper half cycle of sine wave.*/ 
      /* This is achieved by  adding sample values into AVERAGE_AMP.*/
      PWM_Value = (uint8_t)(AVERAGE_AMP + sin.Value[sin.Index]);
    }
    else 
    {
      /* Lower half cycle of sine wave */ 
      /* Load the duty cycle register for lower  half cycle of sine wave. */ 
      /* This is achieved by   subtracting sample values into AVERAGE_AMP. */    
      PWM_Value = (uint8_t)(AVERAGE_AMP - sin.Value[sin.Index]);
    }
    TIM1_SetCompare2(PWM_Value);
    /*Increment the index for pointing to wardsnext sample value to be loaded */
    sin.Index++;
    if (sin.Index == nbr_of_samples_half) 
    {
      /* Reset the index value */
      sin.Index = 0;
      /* Lower half sin wave is activated if previously it was upper half and vice-versa */
      sin.Up = (bool)(~sin.Up);
    } 
  }
  /* Increment the counter value at each overflow interrupt */
  counter++;

  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
	// Wake-up /trigg the ADC 
	ADC1_Cmd(ENABLE);
	// clear compare flag
	TIM1_ClearITPendingBit(TIM1_IT_CC1);
}

#ifdef STM8S903
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/**
* @brief Timer2 Update/Overflow/Break Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief Timer2 Capture/Compare Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208 or STM8S207 or STM8S103 or STM8S903 or STM8AF62Ax or STM8AF52Ax */

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /* STM8S105 or STM8AF626x */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#else /*STM8S105, STM8S103 or STM8S903 or STM8AF626x */
/**
* @brief ADC1 interrupt routine.
* @par Parameters:
* None
* @retval
* None
*/
INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  if(ADInit == FALSE  &&  ADSampRdy == FALSE)
	{
		temp_AD_H = ADC1->DRH;	       // read conversion result (MSB first!)
		temp_AD_L = ADC1->DRL;	       // and store it to AD samples field
		AD_sample[AD_samp]= ((u16)(temp_AD_H) << 2) | (temp_AD_L & 3);
		//AD_sample[AD_samp]= ADC1_GetConversionValue();
		if(++AD_samp >= NUMB_SAMP)	 // AD_smaple field is full?
			ADSampRdy= TRUE;				   // YES - set field ready flag for main loop
	}
	else
		ADInit= FALSE;					     // NO - ignore sample, wait for next one
		
	// clear end of conversion flag
	ADC1_ITConfig(ADC1_IT_EOC, DISABLE);
	// stop ADC
	ADC1_Cmd(DISABLE);
}
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#ifdef STM8S903
/**
  * @brief Timer6 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
* @brief Timer4 Update/Overflow Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  /* Update remaining Delay every 1 ms interrupt*/
  UpdateTimers()
  
	TimingDelay_Decrement();
	
	/* count ms*/
  ms++;
  if (ms >= 10)
    {
      ms = 0;
			
      /* count ten ms*/
      ten_ms++;
      if (ten_ms >= 100)
      {
        ten_ms = 0;
        /* count seconds*/
        sec++;
        /* if 1 second elapsed then event flag true*/
        event = TRUE;
        /* LD1 toggles every second*/
        //Toggle();
        if (sec >= 60)
        {
          sec = 0;
          /* count min*/
          min++;
          if (min >= 60)
            min = 0;
        }
      }
    }

  /* Cleat Interrupt Pending bit */
  TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
}
#endif /*STM8S903*/

/**
  * @brief Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
