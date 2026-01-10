/*
 * uart.h
 *
 *  Created on: Dec 19, 2025
 *      Author: brajo
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "stm32f030x8.h"

#define  	ISR_TCIF4		(1U<<13)
#define 	IFCR_CTCIF4		(1U<<13)

/* Initialize USART2 TX on PA2 */
void uart2_tx_init(uint32_t baudrate);

/* Initialize USART2 RX & TX on PA2 */
void uart2_rxtx_init(uint32_t baudrate);

/*read RX data*/
char uart2_read(void);

void uart2_rx_interrupt_init(uint32_t baudrate);

void dma_uart2_tx_init(uint32_t src,uint32_t dst,uint32_t len);

/* Transmit one character over USART2 */
void uart2_write(char ch);

#endif /* UART_H_ */
