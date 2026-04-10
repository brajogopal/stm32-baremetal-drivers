#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include <stdio.h>

void jmp_to_default_app(void);

typedef struct
{
    void (*delay_ms)(uint32_t tim);
    void (*delay)(uint32_t sec);
    void (*pinMode)(pin_t pin, mode_t mode);
    void (*digitalWrite)(pin_t pin, uint8_t state);
} btl_common_apis;

__attribute__((section(".COMMON_APIS"))) const btl_common_apis common_api_table = {
		.delay_ms = delay_ms,
		.delay = delay,
		.pinMode = pinMode,
		.digitalWrite = digitalWrite


};
int main() {
	debug_uart_init(9600);
	jmp_to_default_app();
	while (1) {
	}
}



void jmp_to_default_app(void) {
	uint32_t app_start_address;
	func_ptr jump_to_app;
	println("bootloader started..\n\r");
	delay_ms(300);

	if (is_valid_app(APPLICATION_ADDRESS)) {

		__disable_irq();
		relocate_vector_table();

		/*Initialize main stack pointer */
		__set_MSP(*(uint32_t*) APPLICATION_ADDRESS);
		__enable_irq();

		app_start_address = *(uint32_t*) (APPLICATION_ADDRESS + 4);
		jump_to_app = (func_ptr) (app_start_address);

		/*jump*/
		jump_to_app();

		while (1);//Fail-safe design(If execution ever comes back, CPU gets stuck here safely)
	} else {
		println("No valid application found");
	}
}
