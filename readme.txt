适合业余爱好者DIY高精度数字电桥 


http://www.crystalradio.cn/thread-231933-131-1.html
http://bbs.38hot.net/read.php?tid=13531
http://www.ourdev.cn/thread-5152439-1-2.html

程序为矿坛下的 gyp2000更改的 1602 V5.5


/**
  @page exampletemplate Project Example Template
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   This sub-directory contains all the user-modifiable files 
  *          needed to create a new project linked with the STM8S/A  
  *          Standard Peripheral Library.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Project Example Template Description 

This example is used as a template that can be used as reference to build any new 
firmware development for all STM8S and STM8A product devices using the STM8S/A 
Standard Peripherals Library.


@par Directory contents 

This folder contains standard template projects for EWSTM8, STVD with Cosmic, 
STVD with raisonance and RIDE toolchains that compile the needed STM8S/A Standard
Peripheral's drivers plus all the user-modifiable files that are necessary to 
create a new project. 

  - Project/led/inc/stm8s_conf.h    Library Configuration file

  - Project/led/src/stm8s_it.c:     Source file containing the interrupt handlers (the 
                                     function bodies are empty in this template).

  - Project/led/inc/stm8s_it.h      Header file including all interrupt handlers prototypes.

  - Project/led/src/main.c          Main program
          
 @note
  - High-Density STM8A devices are the STM8AF52xx STM8AF6269/8x/Ax,
    STM8AF51xx, and STM8AF6169/7x/8x/9x/Ax microcontrollers where the Flash memory
    density ranges between 32 to 128 Kbytes
  - Medium-Density STM8A devices are the STM8AF622x/4x, STM8AF6266/68,
    STM8AF612x/4x, and STM8AF6166/68 microcontrollers where the Flash memory 
    density ranges between 8 to 32 Kbytes
  - High-Density STM8S devices are the STM8S207xx, STM8S007 and STM8S208xx microcontrollers
    where the Flash memory density ranges between 32 to 128 Kbytes.
  - Medium-Density STM8S devices are the STM8S105x and STM8S005 microcontrollers
    where the Flash memory density ranges between 16 to 32-Kbytes.
  - Low-Density STM8S devices are the STM8S103xx, STM8S003 and STM8S903xx microcontrollers
    where the Flash density is 8 Kbytes.
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */

