/*
 * main.h
 *
 *  Created on: 03 дек. 2014 г.
 *      Author: Nik
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"



#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

#define N 500
#define ADC_N N

#define DAC_N (N/5)
#define OSR 20
#define ADC_CHANGEOVR (N)

#define SINE_OFFSET (2047)

#define IRQ_ADC_SAMPLE 1
#define IRQ_DAC_INVERSE 2
#define IRQ_DAC_ZERO 4
#define IRQ_ADC_WAIT 8

#define cordic_1K 0x26DCEDB0
#define half_pi 0x40000000
#define MUL 1073741824.000000
#define CORDIC_NTAB 32

#define ALPHA 0.05
#define FLT_LEN 4

#define SHUNT 150.0
#define CAL_ROUNDS 60
#define CAL_SETTLE 6

#define PWR_BTN 0
#define SP_BTN 1
#define REL_BTN 2

#define PUSH_MSK 0x000003
#define PUSH_MAP 0x000002
#define LPUSH_MSK 0x0000003F
#define LPUSH_MAP 0x0000001F
#define RPT_MSK 0x000000FF
#define RPT_MAP 0x000000FF

#define BTN_PUSH  0x000001
#define BTN_LPUSH 0x000100
#define BTN_RPT   0x010000
#define BTN_CNT   3

// 4min vs 30min
#define APO_4MIN 2400
#define APO_MAX  18000

#define VREF_CH 3
#define BAT_CH 4
#define BAT_SKIP 30
#define CH_NUM 5

#define BAT_MIN 3200
#define BAT_MAX 4200
#define BAT_N_SEG 6

#define DEF_CONTRAST 12
#define DEF_AVERAGES 5
#define MAX_MENU_ITEMS 6

#define EEP_PARAMS1 1
#define EEP_SHUNT 	2
#define EEP_BALANCE_BASE 	0x20
#define EEP_CAL_BASE 		0x40
#define EEP_SHIFT_BASE		0x60
// ----------------------------
// functions
int buttons();
void powerOff(void);
void sputdec(char * buf,int inp);
void putdec(int inp);
void setBacklight(int mode);
// ----------------------------
// types
typedef struct complex_number {
	float Re;
	float Im;
} cplx;
//----------------------------------------------------------------------------
typedef struct
{
__IO uint32_t* port;
unsigned int msk;
} buttons_t;
//----------------------------------------------------------------------------
typedef struct
{
cplx Zpolar;
cplx polar[3];
float Q,D;

} measure_t;

typedef struct
{
cplx shift0;
cplx shift1;
cplx shift2;

cplx Corr0;
cplx Corr2;
//cplx sCorr[3];
} balance_data_t;
//----------------------------------------------------------------------------
typedef struct
{
cplx Zo;
cplx Zs;
} cal_data_t;
//----------------------------------------------------------------------------
typedef int (*menuHandler)(int);
typedef int (*menuParamEdit)(int, char *, int); //idx, out string,buttons

typedef const struct
{
menuHandler m;
menuParamEdit d;
const int8_t parent;
const int8_t first;
const int8_t last;
const char * name;
const int32_t timeout;
} menuItem;

typedef const struct
{
uint32_t * val;
const char** list;
const int32_t startBit;
const int32_t lenBits;
const int32_t max;
} bitField;

//----------------------------------------------------------------------------
#endif /* MAIN_H_ */
