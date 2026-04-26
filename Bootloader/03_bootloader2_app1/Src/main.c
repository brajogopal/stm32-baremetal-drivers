#include "stm32f030x8.h"
#include "uart.h"
#include "common_api.h"
#include <stdio.h>

#define COMMON_API_ADDR		0x08006000



int main(void){
	debug_uart_init(9600);
	const btl_common_apis *common_apis = (const btl_common_apis*) COMMON_API_ADDR;

	common_apis->pinMode(PA5,OUTPUT);

	while(1){
		println("Application new Running..");
		common_apis->digitalWrite(PA5,1);
		common_apis->delay_ms(500);
		common_apis->digitalWrite(PA5,0);
		common_apis->delay_ms(500);
	}
}
