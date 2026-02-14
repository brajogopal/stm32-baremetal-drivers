#include "stm32f030x8.h"
#include "delay.h"
#include <stdio.h>

#define GPIOAEN       (1U<<17)



#define APPLICATION_ADDRESS			0x08002000

typedef void(*func_ptr)(void);

void jmp_to_default_app(void){
	uint32_t app_start_address;	//32 bit local variable
	func_ptr jump_to_app;

	printf("Bootloader Started ...\n\r");
	delay_ms(300);

	/*Initialize main stack pointer*/
	__set_MSP(*(uint32_t *)APPLICATION_ADDRESS);

	app_start_address = *(uint32_t *)(APPLICATION_ADDRESS + 4); //Read Reset_Hamdler address

	jump_to_app = (func_ptr) app_start_address;	//store that address in a Function pointer

	//jump
	jump_to_app();
}

int main(){

	while(1){
	}
}
