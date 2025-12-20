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

/* Initialize USART2 RX & TX on PA2 */
void uart2_rxtx_init(uint32_t baudrate);

/*read RX data*/
char uart2_read(void);

/* Transmit one character over USART2 */
void uart2_write(char ch);

#endif /* UART_H_ */
