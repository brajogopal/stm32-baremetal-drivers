

#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include <stdio.h>




int main(void){
	pinMode(PA5,OUTPUT);
	debug_uart_init(9600);

	while(1){
		println("Application 1  Running..");
		digitalWrite(PA5,HIGH);
		delay_ms(500);
		digitalWrite(PA5,LOW);
		delay_ms(500);
	}
}
