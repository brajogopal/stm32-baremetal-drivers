/*
 * uart.h
 *
 *  Created on: Dec 19, 2025
 *      Author: brajo
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/* Initialize USART2 TX on PA2 */
void uart2_tx_init(uint32_t baudrate);

/* Transmit one character over USART2 */
void uart2_write(char ch);

uint32_t get_pclk1_freq(void);

#endif /* UART_H_ */
