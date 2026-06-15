/*
 * uart.h
 *
 *  Created on: 10-Feb-2026
 *      Author: brajo
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f030x8.h"

#define UART_RXNE	(1U<<5)

typedef enum{
    UART_OK,
    UART_TIMEOUT,
    UART_ERROR
}uart_status_t;


void debug_uart_init(uint32_t baudrate);
void uart_interrupt_init(uint32_t baudrate);
void println(const char *str);
char uart_receive_char(void);
void uart_receive_string(char *buffer, uint32_t max_length);
uart_status_t uart_receive_with_timeout(uint8_t *data,uint32_t timeout);

#endif /* UART_H_ */
