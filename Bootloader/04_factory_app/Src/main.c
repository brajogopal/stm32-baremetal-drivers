

#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include <stdio.h>




int main(void){

	debug_uart_init(9600);

	while(1){
		println("Factory Application Running..");
		delay_ms(500);
	}
}
