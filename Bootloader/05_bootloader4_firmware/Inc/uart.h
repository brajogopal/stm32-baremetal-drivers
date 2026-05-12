/*
 * uart.h
 *
 *  Created on: 07-Apr-2026
 *      Author: BROJOGOPAL
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f030x8.h"

void debug_uart_init(uint32_t baudrate);
void println(const char *str);
void uart_write(char ch);

#endif /* UART_H_ */
