


#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

void __attribute__ ((weak)) _init(void)  {};

#define SYSCLK_FREQ  32000000

//uint32_t SystemCoreClock         = SYSCLK_FREQ;        /*!< System Clock Frequency (Core Clock) */

void SystemInit (void)
{
	/* Reset the RCC clock configuration to the default reset state(for debug purpose) */
	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;
	/* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CFGR &= (uint32_t)(~(RCC_CFGR_HPRE|RCC_CFGR_PPRE2|RCC_CFGR_PPRE1|RCC_CFGR_PLLSRC|RCC_CFGR_PLLMULL|RCC_CFGR_PLLXTPRE|RCC_CFGR_SW|RCC_CFGR_ADCPRE|RCC_CFGR_MCO));
	RCC->CR &= (uint32_t)(~(RCC_CR_CSSON|RCC_CR_HSEON|RCC_CR_HSEBYP|RCC_CR_PLLON));

	RCC->CIR = 0x009F0000;	/* Disable all interrupts and clear pending bits  */
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);	while( (RCC->CR & RCC_CR_HSERDY) == 0);

	RCC->CFGR |=  RCC_CFGR_PLLSRC |RCC_CFGR_PLLMULL8;//8
	RCC->CFGR2 = 2; // 12M / 3 * 8 = 32
	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;	while((RCC->CR & RCC_CR_PLLRDY) == 0) {}

	RCC->CFGR |= (RCC->CFGR & (~RCC_CFGR_SW))|(uint32_t)RCC_CFGR_SW_PLL;//(uint32_t)RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL) {}


}
