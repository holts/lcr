/**
  ******************************************************************************
  * @file    stm8_tsl_conf_RC_TOADAPT.h
  * @author  MCD Application Team
  * @version V2.4.0
  * @date    01-July-2011
  * @brief   STM8 Touch Sensing Library - This file is a template for the
  *          RC acquisition and must be adapted to your requirements.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TSL_CONF_H
#define __TSL_CONF_H


//==============================================================================
//
// 1) MCU SELECTION
//
// 1 = STM8L10x
// 2 = STM8L15x with 4Kb or 8Kb Flash (low density)
// 3 = STM8L15x with 16Kb, 32kb or 64Kb Flash (medium and high density)
//     STM8L16x
// 4 = STM8S
//
//==============================================================================

#define MCU_SELECTION  (4)


//==============================================================================
//
// 2) ACQUISITION TIMER SELECTION (TIMACQ)
//
// Set the acquisition timer name and its counter high register address.
// The timer you select must be a 16-bit timer, have a 8-bit prescaler and
// must be different of the TIMTICK timer described below.
// This table summarizes the values for some products (consult the datasheet
// if your product is not in the list):
// Product                TIMACQ        TIMACQ_CNTR_ADD
// STM8S20x               TIM2          0x530A
//     "                  TIM3          0x5328
// STM8S105               TIM2          0x530A
//     "                  TIM3          0x5328
// STM8S103               TIM2          0x530C
// STM8L10x               TIM2          0x525B
//     "                  TIM3          0x528B
// STM8L15x               TIM2          0x525C
//     "                  TIM3          0x528C
//
//==============================================================================

#define TIMACQ           (TIM2)
#define TIMACQ_CNTR_ADD  (0x530A)


//==============================================================================
//
// 3) GENERIC TIMEBASE TIMER SELECTION (TIMTICK)
//
// Set the generic timebase timer.
// The timer you select must be a basic 8-bit timer and must be different
// of the TIMACQ timer described above.
// This table summarizes the values for some products (consult the datasheet
// if your product is not in the list):
// Product                TIMTICK
// STM820x                TIM4
// STM8S105               TIM4
// STM8S103               TIM4
// STM8L10x               TIM4
// STM8L15x               TIM4
// Warning: The selected timer update/overflow interrupt vector must point to
// the TSL_Timer_ISR() interrupt routine.
//
//==============================================================================

#define TIMTICK         (TIM4)


//==============================================================================
//
// 4) REFERENCE LOAD I/O DEFINITION
//
// Set the port
// Set the pin mask
//
//======================================================================

#define LOADREF_PORT_ADDR  (GPIOA_BaseAddress)  /**< LOADREF pin GPIO base address */

#define LOADREF_BIT        (0x08)               /**< LOADREF pin mask */


//======================================================================
//
// 5) SINGLE CHANNEL KEYS DEFINITION - PORT 1
//
// Set the number of keys
// Set the port
// Set the pins mask
//
// Warning: This port is mandatory and one key at least must be defined.
//
//=======================================================================

#define SCKEY_P1_KEY_COUNT  (3)  /**< Single channel key Port 1: Number of keys used (value from 1 to 8) */

#define SCKEY_P1_PORT_ADDR  (GPIOC_BaseAddress)  /**< Single channel key Port 1: GPIO base address */

#define SCKEY_P1_A  (0x80)  /**< Single channel key Port 1: 1st key mask */
#define SCKEY_P1_B  (0x40)  /**< Single channel key Port 1: 2nd key mask */
#define SCKEY_P1_C  (0x20)  /**< Single channel key Port 1: 3rd key mask */
#define SCKEY_P1_D  (0)     /**< Single channel key Port 1: 4th key mask */
#define SCKEY_P1_E  (0)     /**< Single channel key Port 1: 5th key mask */
#define SCKEY_P1_F  (0)     /**< Single channel key Port 1: 6th key mask */
#define SCKEY_P1_G  (0)     /**< Single channel key Port 1: 7th key mask */
#define SCKEY_P1_H  (0)     /**< Single channel key Port 1: 8th key mask */

#define SCKEY_P1_DRIVEN_SHIELD_MASK (0x10)


//=========================================================================
//
// 6) SINGLE CHANNEL KEYS DEFINITION - PORT 2
//
// Set the number of keys
// Set the port
// Set the pins mask
//
// Note: This port is optional. Set SCKEY_P2_KEY_COUNT to 0 to not use this port.
//
//==========================================================================

#define SCKEY_P2_KEY_COUNT  (0)  /**< Single channel key Port 2: Number of keys used (value from 0 to 8) */

#define SCKEY_P2_PORT_ADDR  (GPIOD_BaseAddress)  /**< Single channel key Port 2: GPIO base address */

#define SCKEY_P2_A  (0x01)  /**< Single channel key Port 2: 1st key mask */
#define SCKEY_P2_B  (0)     /**< Single channel key Port 2: 2nd key mask */
#define SCKEY_P2_C  (0)     /**< Single channel key Port 2: 3rd key mask */
#define SCKEY_P2_D  (0)     /**< Single channel key Port 2: 4th key mask */
#define SCKEY_P2_E  (0)     /**< Single channel key Port 2: 5th key mask */
#define SCKEY_P2_F  (0)     /**< Single channel key Port 2: 6th key mask */
#define SCKEY_P2_G  (0)     /**< Single channel key Port 2: 7th key mask */
#define SCKEY_P2_H  (0)     /**< Single channel key Port 2: 8th key mask */

#define SCKEY_P2_DRIVEN_SHIELD_MASK (0x00)


//==============================================================================
//
// 7) SINGLE CHANNEL KEYS DEFINITION - PORT 3
//
// Set the number of keys
// Set the port
// Set the pins mask
//
// Note: This port is optional. Set SCKEY_P3_KEY_COUNT to 0 to not use this port.
//
//==============================================================================

#define SCKEY_P3_KEY_COUNT  (0)  /**< Single channel key Port 3: Number of keys used (value from 0 to 8) */

#define SCKEY_P3_PORT_ADDR  (GPIOE_BaseAddress)  /**< Single channel key Port 3: GPIO base address */

#define SCKEY_P3_A  (0x80)  /**< Single channel key Port 3: 1st key mask */
#define SCKEY_P3_B  (0)     /**< Single channel key Port 3: 2nd key mask */
#define SCKEY_P3_C  (0)     /**< Single channel key Port 3: 3rd key mask */
#define SCKEY_P3_D  (0)     /**< Single channel key Port 3: 4th key mask */
#define SCKEY_P3_E  (0)     /**< Single channel key Port 3: 5th key mask */
#define SCKEY_P3_F  (0)     /**< Single channel key Port 3: 6th key mask */
#define SCKEY_P3_G  (0)     /**< Single channel key Port 3: 7th key mask */
#define SCKEY_P3_H  (0)     /**< Single channel key Port 3: 8th key mask */

#define SCKEY_P3_DRIVEN_SHIELD_MASK (0x00)


//==============================================================================
//
// 8) NUMBER OF MULTI CHANNEL KEYS AND NUMBER OF CHANNELS USED
//
// Set the total number of multi channel keys used (0, 1 or 2)
// Set the number of channels (5 or 8)
//
//==============================================================================

#define NUMBER_OF_MULTI_CHANNEL_KEYS  (0)  /**< Number of multi channel keys (value from 0 to 2) */
#define CHANNEL_PER_MCKEY             (5)  /**< Number of channels per key (possible values are 5 or 8 only) */


//==============================================================================
//
// 9) MULTI CHANNEL KEY 1 DEFINITION
//
// Set the port used
// Set the pins mask
//
// Note: This key is optional
//
//==============================================================================

#if NUMBER_OF_MULTI_CHANNEL_KEYS > 0

#define MCKEY1_A_PORT_ADDR  (GPIOD_BaseAddress)  /**< Multi channel key 1: 1st channel port */
#define MCKEY1_A            (0x40)               /**< Multi channel key 1: 1st channel mask */
#define MCKEY1_B_PORT_ADDR  (GPIOD_BaseAddress)  /**< Multi channel key 1: 2nd channel port */
#define MCKEY1_B            (0x20)               /**< Multi channel key 1: 2nd channel mask */
#define MCKEY1_C_PORT_ADDR  (GPIOD_BaseAddress)  /**< Multi channel key 1: 3rd channel port */
#define MCKEY1_C            (0x10)               /**< Multi channel key 1: 3rd channel mask */
#define MCKEY1_D_PORT_ADDR  (GPIOD_BaseAddress)  /**< Multi channel key 1: 4th channel port */
#define MCKEY1_D            (0x08)               /**< Multi channel key 1: 4th channel mask */
#define MCKEY1_E_PORT_ADDR  (GPIOD_BaseAddress)  /**< Multi channel key 1: 5th channel port */
#define MCKEY1_E            (0x04)               /**< Multi channel key 1: 5th channel mask */
#define MCKEY1_F_PORT_ADDR  (0)                  /**< Multi channel key 1: 6th channel port */
#define MCKEY1_F            (0)                  /**< Multi channel key 1: 6th channel mask */
#define MCKEY1_G_PORT_ADDR  (0)                  /**< Multi channel key 1: 7th channel port */
#define MCKEY1_G            (0)                  /**< Multi channel key 1: 7th channel mask */
#define MCKEY1_H_PORT_ADDR  (0)                  /**< Multi channel key 1: 8th channel port */
#define MCKEY1_H            (0)                  /**< Multi channel key 1: 8th channel mask */

#define MCKEY1_TYPE (0)                          /**< Multi channel key 1 type: 0=wheel (zero between two electrodes), 1=slider (zero in the middle of one electrode) */
#define MCKEY1_LAYOUT_TYPE (0)                   /**< Multi channel key 1 layout type: 0=interlaced, 1=normal */

#define MCKEY1_DRIVEN_SHIELD_MASK (0x00)

#endif


//==============================================================================
//
// 10) MULTI CHANNEL KEY 2 DEFINITION
//
// Set the port used
// Set the pins mask
//
// Note: This key is optional.
//
//==============================================================================

#if NUMBER_OF_MULTI_CHANNEL_KEYS > 1

#define MCKEY2_A_PORT_ADDR  (GPIOE_BaseAddress)  /**< Multi channel key 2: 1st channel port */
#define MCKEY2_A            (0x01)               /**< Multi channel key 2: 1st channel mask */
#define MCKEY2_B_PORT_ADDR  (GPIOE_BaseAddress)  /**< Multi channel key 2: 2nd channel port */
#define MCKEY2_B            (0x02)               /**< Multi channel key 2: 2nd channel mask */
#define MCKEY2_C_PORT_ADDR  (GPIOE_BaseAddress)  /**< Multi channel key 2: 3rd channel port */
#define MCKEY2_C            (0x04)               /**< Multi channel key 2: 3rd channel mask */
#define MCKEY2_D_PORT_ADDR  (GPIOE_BaseAddress)  /**< Multi channel key 2: 4th channel port */
#define MCKEY2_D            (0x08)               /**< Multi channel key 2: 4th channel mask */
#define MCKEY2_E_PORT_ADDR  (GPIOE_BaseAddress)  /**< Multi channel key 2: 5th channel port */
#define MCKEY2_E            (0x10)               /**< Multi channel key 2: 5th channel mask */
#define MCKEY2_F_PORT_ADDR  (0)                  /**< Multi channel key 2: 6th channel port */
#define MCKEY2_F            (0)                  /**< Multi channel key 2: 6th channel mask */
#define MCKEY2_G_PORT_ADDR  (0)                  /**< Multi channel key 2: 7th channel port */
#define MCKEY2_G            (0)                  /**< Multi channel key 2: 7th channel mask */
#define MCKEY2_H_PORT_ADDR  (0)                  /**< Multi channel key 2: 8th channel port */
#define MCKEY2_H            (0)                  /**< Multi channel key 2: 8th channel mask */

#define MCKEY2_TYPE (0)                          /**< Multi channel key 2 type: 0=wheel (zero between two electrodes), 1=slider (zero in the middle of one electrode) */
#define MCKEY2_LAYOUT_TYPE (0)                   /**< Multi channel key 2 layout type: 0=interlaced, 1=normal */

#define MCKEY2_DRIVEN_SHIELD_MASK (0x00)

#endif


//==============================================================================
//
// 11) ELECTRODES MASKS USED ON EACH GPIO
//
// Define the electrodes mask for each GPIO used (SCKeys + MCKeys but not LOADREF)
//
//==============================================================================

#define GPIOA_ELECTRODES_MASK  (0x00)  /**< Electrodes mask for GPIOA */
#define GPIOB_ELECTRODES_MASK  (0x00)  /**< Electrodes mask for GPIOB */
#define GPIOC_ELECTRODES_MASK  (0x8E)  /**< Electrodes mask for GPIOC */
#define GPIOD_ELECTRODES_MASK  (0x7D)  /**< Electrodes mask for GPIOD */
#define GPIOE_ELECTRODES_MASK  (0x00)  /**< Electrodes mask for GPIOE */
#define GPIOF_ELECTRODES_MASK  (0x00)  /**< Electrodes mask for GPIOF */
#define GPIOG_ELECTRODES_MASK  (0x00)  /**< Electrodes mask for GPIOG */
#define GPIOH_ELECTRODES_MASK  (0x00)  /**< Electrodes mask for GPIOH */
#define GPIOI_ELECTRODES_MASK  (0x00)  /**< Electrodes mask for GPIOI */


//============================================================================
//
// 12) TSL PARAMETERS CONFIGURATION
//
//============================================================================

/** @addtogroup TSL_parameters_RC
  * @{ */

// IO acquisition
#define SCKEY_ACQ_NUM                   (3)  /**< Single channel key acquisition number - N (value from 1 to 255) */
#define SCKEY_ADJUST_LEVEL              (1)  /**< Single channel key adjustment level (value from 0 to 255) */
#define MCKEY_ACQ_NUM                   (6)  /**< Multi channel key acquisition number - N (value from 1 to 255) */
#define MCKEY_ADJUST_LEVEL              (1)  /**< Multi channel key adjustment level (value from 0 to 255) */

// IO acquisition number of rejected values and measure guardbands
#define MAX_REJECTED_MEASUREMENTS       (5)  /**< Max number of rejected measurements allowed (value from 0 to 255) */
#define MAX_MEAS_COEFF             (0x011A)  /**< Max measure guardband (MSB=integer part, LSB=decimal part) */
#define MIN_MEAS_COEFF             (0x00E6)  /**< Min measure guardband (MSB=integer part, LSB=decimal part) */

// Thresholds
#define SCKEY_DETECTTHRESHOLD_DEFAULT          (10)  /**< Single channel key detection threshold (value from 1 to 127) */
#define SCKEY_ENDDETECTTHRESHOLD_DEFAULT        (8)  /**< Single channel key end detection threshold (value from 1 to 127) */
#define SCKEY_RECALIBRATIONTHRESHOLD_DEFAULT  (-10)  /**< Single channel key calibration threshold (value from -1 to -128) */
#define MCKEY_DETECTTHRESHOLD_DEFAULT          (30)  /**< Multi channel key detection threshold (value from 1 to 127) */
#define MCKEY_ENDDETECTTHRESHOLD_DEFAULT       (20)  /**< Multi channel key end detection threshold (value from 1 to 127) */
#define MCKEY_RECALIBRATIONTHRESHOLD_DEFAULT  (-30)  /**< Multi channel key calibration threshold (value from -1 to -128) */

// MCKey resolution
#define MCKEY_RESOLUTION_DEFAULT                   (4)  /**< Multi channel key resolution (value from 1 to 8) */

// MCKey Direction Change process
#define MCKEY_DIRECTION_CHANGE_ENABLED               (1)  /**< Multi channel key direction change enable (1) or disable (0) switch */
#define MCKEY_DIRECTION_CHANGE_MAX_DISPLACEMENT    (255)  /**< Multi channel key direction change maximum displacement (value from 0 to 255) */
#define MCKEY_DIRECTION_CHANGE_INTEGRATOR_DEFAULT    (1)  /**< Multi channel key direction change integrator (value from 1 to 255) */
#define MCKEY_DIRECTION_CHANGE_THRESHOLD_DEFAULT    (10)  /**< Multi channel key direction change threshold (value from 1 to 255) */

// Integrators
#define DETECTION_INTEGRATOR_DEFAULT       (2)  /**< Detection Integrator = Debounce Filter (value from 0 to 255) */
#define END_DETECTION_INTEGRATOR_DEFAULT   (2)  /**< End detection Integrator = Debounce Filter (from 0 to 255) */
#define RECALIBRATION_INTEGRATOR_DEFAULT  (10)  /**< Calibration integrator (value from 1 to 255) */

// IIR Filter
#define ECS_TIME_STEP_DEFAULT  (20)  /**< Sampling frequency, multiple of 10ms */
#define ECS_TEMPO_DEFAULT      (20)  /**< Delay after detection, multiple of 100ms */
#define ECS_IIR_KFAST_DEFAULT  (20)  /**< K factor for fast filtering */
#define ECS_IIR_KSLOW_DEFAULT  (10)  /**< K factor for slow filtering */

// Detection Timeout
#define DTO_DEFAULT  (0)  /**< 1s unit (value from 0 (= infinite!) to 255) */

// Automatic Calibration
#define NEGDETECT_AUTOCAL (1)  /**< 0 (Enable negative threshold for noise), 1 (Enable autocalibration)  */

// Acquisition values limits
#define SCKEY_MIN_ACQUISITION    (50) /**< Single channel key minimum acquisition value */
#define SCKEY_MAX_ACQUISITION  (3000) /**< Single channel key maximum acquisition value */
#define MCKEY_MIN_ACQUISITION   (150) /**< Multi channel key minimum acquisition value */
#define MCKEY_MAX_ACQUISITION  (5000) /**< Multi channel key maximum acquisition value */

// Optional parameters for Delta Normalization Process (for Multi channel keys only).
// The MSB is the integer part, the LSB is the real part:
// For example to apply a factor 1.10:
// 0x01 to the MSB
// 0x1A to the LSB (0.1 x 256 = 25.6 -> 26 = 0x1A)
// Final value to define is: 0x011A

#define MCKEY1_DELTA_COEFF_A (0x0100) /**< MCKey1 Channel A parameter */
#define MCKEY1_DELTA_COEFF_B (0x0100) /**< MCKey1 Channel B parameter */
#define MCKEY1_DELTA_COEFF_C (0x0100) /**< MCKey1 Channel C parameter */
#define MCKEY1_DELTA_COEFF_D (0x0100) /**< MCKey1 Channel D parameter */
#define MCKEY1_DELTA_COEFF_E (0x0100) /**< MCKey1 Channel E parameter */
#define MCKEY1_DELTA_COEFF_F (0x0100) /**< MCKey1 Channel F parameter */
#define MCKEY1_DELTA_COEFF_G (0x0100) /**< MCKey1 Channel G parameter */
#define MCKEY1_DELTA_COEFF_H (0x0100) /**< MCKey1 Channel H parameter */

#define MCKEY2_DELTA_COEFF_A (0x0100) /**< MCKey2 Channel A parameter */
#define MCKEY2_DELTA_COEFF_B (0x0100) /**< MCKey2 Channel B parameter */
#define MCKEY2_DELTA_COEFF_C (0x0100) /**< MCKey2 Channel C parameter */
#define MCKEY2_DELTA_COEFF_D (0x0100) /**< MCKey2 Channel D parameter */
#define MCKEY2_DELTA_COEFF_E (0x0100) /**< MCKey2 Channel E parameter */
#define MCKEY2_DELTA_COEFF_F (0x0100) /**< MCKey2 Channel F parameter */
#define MCKEY2_DELTA_COEFF_G (0x0100) /**< MCKey2 Channel G parameter */
#define MCKEY2_DELTA_COEFF_H (0x0100) /**< MCKey2 Channel H parameter */

// Interrupt synchronisation
#define IT_SYNC  (1) /**< Interrupt synchronisation. (=1) Allow to synchronize the aquisition with a flag set in an interrupt routine */

// Spread spectrum
#define SPREAD_SPECTRUM      (1) /**< Spread spectrum. (=1) Add a variable delay between acquisitions */
#define SPREAD_COUNTER_MIN   (1) /**< Spread min value */
#define SPREAD_COUNTER_MAX  (20) /**< Spread max value */

// RTOS Management of the acquisition (instead of the timebase interrupt sub-routine
#define RTOS_MANAGEMENT    (0) /**< The Timebase routine is launched by the application instead to be managed through a timebase interrupt routine */
// Timer Callback to allow the user to add its own function called from the timer interrupt sub-routine
#define TIMER_CALLBACK (0)    /**< if (1) Allows the use of a callback function in the timer interrupt. This function will be called every 0.5ms. The callback function must be defined inside the application and have the following prototype FAR void USER_TickTimerCallback(void);  */
//Inline functions
#define USE_INLINED_FUNCTIONS (0) /**< Inline functions are enabled (=1) */
/** @} */


//==============================================================================
//
// DEFINITIONS CHECK. DO NOT TOUCH ANYTHING BELOW !!!
//
//==============================================================================

#include "stm8_tsl_checkconfig.h"

#endif /* __TSL_CONF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
