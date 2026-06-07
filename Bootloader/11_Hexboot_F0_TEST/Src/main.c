

#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include <stdio.h>




int main(void){
	pinMode(PA5,OUTPUT);
	systick_init();
	debug_uart_init(9600);

	while(1){
		println("Application new Running..\n\r");
		digitalWrite(PA5,HIGH);
		systick_delay(500);
		digitalWrite(PA5,LOW);
		systick_delay(500);
	}
}
