/**
 * @file           : main.c
 * @author         : By BrajoGopal
 * @brief          : Main program body
 */

#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include <stdio.h>

#define CUSTOM_FUNC  __attribute__((section(".custom_section")))

void CUSTOM_FUNC led_toggle(uint32_t dly){
		digitalWrite(PA5,HIGH);
		delay_ms(dly);
		println("LED GLOWING");
		digitalWrite(PA5,LOW);
		delay_ms(dly);
}

int main(){

	debug_uart_init(9600);
	pinMode(PA5,OUTPUT);

	while(1){
		led_toggle(500);
	}
}
