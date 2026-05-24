/*
 * uart.h
 *
 *  Created on: 10-Feb-2026
 *      Author: brajo
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f030x8.h"

void debug_uart_init(uint32_t baudrate);
void println(const char *str);
char uart_receive_char(void);
void uart_receive_string(char *buffer, uint32_t max_length);

#endif /* UART_H_ */
