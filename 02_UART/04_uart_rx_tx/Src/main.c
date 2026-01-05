/*
 * main.c
 *
 * Simple application demonstrating USART2 TX & RX
 */

#include 	<stdio.h>
#include 	"uart.h"




#define UART_BAUDRATE	115200
volatile char key;


int main (void){

    /* Initialize USART2 at 115200 baud */
    uart2_rxtx_init(UART_BAUDRATE);

	while(1){
		key = uart2_read();

			printf("you pressed %c\r\n",key);

	}
}
