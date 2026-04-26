#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include "common_api.h"
#include <stdio.h>



__attribute__((section(".COMMON_APIS"))) const btl_common_apis common_api_table = {
		.delay_ms = delay_ms,
		.delay = delay,
		.pinMode = pinMode,
		.digitalWrite = digitalWrite,
		.togglePin = togglePin,
		.digitalRead = digitalRead
};

int main() {
	debug_uart_init(9600);
	jmp_to_default_app();
	while (1) {
	}	}

