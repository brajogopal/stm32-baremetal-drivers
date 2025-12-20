/*
 * main.c
 *
 * Simple application demonstrating USART2 TX using printf
 */

#include 	<stdio.h>
#include 	"uart.h"

#define UART_BAUDRATE	115200

int main (void){

    /* Initialize USART2 at 115200 baud */
    uart2_tx_init(UART_BAUDRATE);

	while(1){
		printf("hello from stm32f030..\r\n");
	}
}
