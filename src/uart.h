/*
 * uart.h
 *
 *  Created on: 26.10.2012
 *      Author: Nik
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#define UART_RX_OVF  (1<<0)
#define UART_RX_LINE (1<<1)
#define UART_TX  	 (1<<2)

#define UART_BUFFER_SIZE 128

#define TRUE (1)
#define FALSE  (0)

void initUART();

//inline void txBufferInit(void);
//inline int txBufferEmpty(void);
//inline int txBufferFull(void);

//inline void rxBufferInit(void);
//inline int rxBufferEmpty(void);
//inline int rxBufferFull(void);

int txPutToBuffer(uint8_t c);
int txGetFromBuffer(uint8_t *c);
int rxPutToBuffer(uint8_t c);
int rxGetFromBuffer(uint8_t *c);

void uartSendByte(uint8_t c);

void uart_tx(const char* str, int wait );
int uart_rx(char * str,int cnt);
int uart_rx_line(char * str,int cnt);

#endif /* UART_H_ */
