/*
 * main.c
 *
 * Simple application demonstrating USART2 TX using printf
 */

#include 	<stdio.h>
#include 	"uart.h"
#include	"rcc_clock.h"

#define UART_BAUDRATE	115200




int main (void){

    /* Initialize USART2 at 115200 baud */
    uart2_tx_init(UART_BAUDRATE);
    uint32_t pclk1 = rcc_get_pclk1_freq();

    printf("pclk1 = %lu\r\n", pclk1);


	while(1){

	}
}
