/*
 * uart.h
 *
 *  Created on: 10-Feb-2026
 *      Author: brajo
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f030x8.h"
#define SR_RXNE		(1U<<5)

void system_uart_init(uint32_t baudrate);
void println(const char *str);


#endif /* UART_H_ */
