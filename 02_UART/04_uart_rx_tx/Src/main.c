/*
 * main.c
 *
 * Simple application demonstrating USART2 TX & RX
 */

#include 	<stdio.h>
#include 	"uart.h"




#define UART_BAUDRATE	115200


int main (void){

	char key;

    /* Initialize USART2 at 115200 baud */
    uart2_rxtx_init(UART_BAUDRATE);

	while(1){
		key = uart2_read();
		if(key == '1'){
			printf("you pressed 1 ..\r\n");
		}else{
			printf("you didn't pressed 1 ..\r\n");
		}
	}
}
