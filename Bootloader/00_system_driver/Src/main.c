/*MODULES -
 * UART
 * GPIO
 * Time-base
 * */


#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include <stdio.h>

#define GPIOAEN       (1U<<17)

int main(){
	debug_uart_init(9600);
	while(1){
		println("Hello from stm32");
		delay(1);
	}
}
