/*
 * main.c
 *
 * Simple application demonstrating USART2 TX using printf
 */

#include 	<stdio.h>
#include 	"uart.h"

#define UART_BAUDRATE	115200

uint32_t pclk;


int main (void){

    /* Initialize USART2 at 115200 baud */
    uart2_tx_init(UART_BAUDRATE);
    pclk = get_pclk1_freq();
	while(1){
		printf("PCLK = %lu\r\n", pclk);
	}
}
