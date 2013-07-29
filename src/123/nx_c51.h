#ifndef _NX_C51_H
#define _NX_C51_H

#include <intrins.h>
#include <reg51.h>

#define __ID_COMBINE(left, right)    (left##right)


#define _SET_BIT(target, bit) ((target) |= (1 << (bit)))
#define _CLR_BIT(target, bit) ((target) &= ~(1 << (bit)))
#define _GET_BIT(target, bit) ((target) & (1 << (bit)))
#define _FLP_BIT(target, bit) ((target) ^= (1 << (bit))) 

#define _BIT(bit) (1 << (bit))
#define BIT(bit)   _BIT(bit)
#define _SET_BITS(target, bits) ((target) |= (bits))
#define _CLR_BITS(target, bits) ((target) &= ~(bits))
#define _GET_BITS(target, bits) ((target) & (bits))
#define _FLP_BITS(target, bits) ((target) ^= (bits)) 


#ifndef NOP
#define NOP(x)              _nop_()
#endif

#define _PIN_HI(port, bit)       (_SET_BIT(port, bit))
#define _PINS_HI(port, bit)       (_SET_BITS(port, bit))

#define _PIN_LO(port, bit)       (_CLR_BIT(port, bit))
#define _PINS_LO(port, bit)       (_CLR_BITS(port, bit))

#define _PIN_GT(port, bit)       (NOP(), _GET_BIT(port, bit))
#define _PINS_GT(port, bit)       (NOP(), _GET_BITS(port, bit))

#define _PIN_FLP(port, bit)      (_FLP_BIT(port, bit))
#define _PINS_FLP(port, bit)      (_FLP_BITS(port, bit))



#define _PxM0(port)             __ID_COMBINE(port, M0)
#define _PxM1(port)             __ID_COMBINE(port, M1)

//推挽输出
#define _PIN_OUT(port, bit)      (_CLR_BIT(_PxM1(port), bit),   _SET_BIT(_PxM0(port), bit))
#define _PINS_OUT(port, bits)    (_CLR_BITS(_PxM1(port), bits), _SET_BITS(_PxM0(port), bits))
//高阻输入
#define _PIN_IN(port, bit)       (_SET_BIT(_PxM1(port), bit),   _CLR_BIT(_PxM0(port), bit))
#define _PINS_IN(port, bits)     (_SET_BITS(_PxM1(port), bits), _CLR_BITS(_PxM0(port), bits))
//准双向（弱上拉，即普通51方式）
#define _PIN_IN_PH(port, bit)    (_CLR_BIT(_PxM1(port), bit),   _CLR_BIT(_PxM0(port), bit))
#define _PINS_IN_PH(port, bits)  (_CLR_BITS(_PxM1(port), bits), _CLR_BITS(_PxM0(port), bits))
//开漏
#define _PIN_OUT_OD(port, bit)   (_SET_BIT(_PxM1(port), bit),  _SET_BIT(_PxM0(port), bit))
#define _PINS_OUT_OD(port, bits) (_SET_BIT(_PxM1(port), bits), _SET_BIT(_PxM0(port), bits))

//#define _BIG_ENDIAN  //C51 is little endian!

#ifdef _BIG_ENDIAN
    #define _L_OF_WORD(x)          (((unsigned char*)&(x))[0])
    #define _H_OF_WORD(x)          (((unsigned char*)&(x))[1])
#else
    #define _L_OF_WORD(x)          (((unsigned char*)&(x))[1])
    #define _H_OF_WORD(x)          (((unsigned char*)&(x))[0])
#endif

//两变量交换
#define _SWAP(a, b)                 ((a) ^= (b), (b) ^= (a), (a) ^= (b))


//数组大小
#define _ARRAY_SIZE(a)    ((sizeof(a)) / (sizeof(a[0])))

//自动外部声明
#ifdef _NOEXTERN
    #define _AUTOEXTERN 
#else
    #define _AUTOEXTERN extern
#endif    

#define NULL                (0)

#define BOOL                bit
#define TRUE                (1)
#define FALSE               (0)



//定时器MODE0模式TH TL值计算

#define _TIME_TO_CYCLES(second)          ((unsigned short)(_MACHINE_FREQUENCY /12.0 * (second)))

//#define _TIMER_VALUE_FROM_TIME(t)     (t * _CRYSTAL / 1000000 / 12)
#define _TIMER0_MODE0_VALUE(second)                             \
                TL0 = ((8192 - _TIME_TO_CYCLES(second))) % 32;	\
                TH0 = (8192 - _TIME_TO_CYCLES(second)) / 32    

#define _TIMER0_MODE1_VALUE(second)                             \
                TL0 = (65536 - _TIME_TO_CYCLES(second)) % 256;  \
                TH0 = (65536 - _TIME_TO_CYCLES(second)) / 256

#define _TIMER0_MODE2_VALUE(second)                             \
                TL0 = (256 - _TIME_TO_CYCLES(second));          \
                TH0 = 256 - _TIME_TO_CYCLES(second)

#define _TIMER1_MODE0_VALUE(second)                             \
                TL1 = (8192 - _TIME_TO_CYCLES(second)) % 32;	\
                TH1 = (8192 - _TIME_TO_CYCLES(second)) / 32    

#define _TIMER1_MODE1_VALUE(second)                             \
                TL1 = (65536 - _TIME_TO_CYCLES(second)) % 256;  \
                TH1 = (65536 - _TIME_TO_CYCLES(second)) / 256

#define _TIMER1_MODE2_VALUE(second)                             \
                TL1 = (256 - _TIME_TO_CYCLES(second));          \
                TH1 = 256 - _TIME_TO_CYCLES(second)


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef signed char SBYTE;
typedef signed short SWORD;
typedef signed long SDWORD;

void DelayCycles(BYTE n);
void Delay1(BYTE n);

BYTE IsHotReset(void);


//void Int0_ISR(void) interrupt 0
//void Timer0_ISR(void) interrupt 1
//void Int1_ISR(void) interrupt 2
//void Timer1_ISR(void) interrupt 3
//void UART1_ISR(void) interrupt 4
//void ADC_ISR(void) interrupt 5
//void LVD_ISR(void) interrupt 6
//void PCA_ISR(void) interrupt 7
//void UART2_ISR(void) interrupt 8
//void SPI_ISR(void) interrupt 9



#include "HardWare.h"
                       
#endif //_NX_C51_H