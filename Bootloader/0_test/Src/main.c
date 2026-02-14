#include "stm32f030x8.h"
#include "delay.h"

#define GPIOAEN       (1U<<17)

#define CUSTOM_FUNC	__attribute__((section(".custom_section")))

unsigned char __attribute__((section(".custom_ram_block"))) custom_ram_buff[10];	//10 bytes

unsigned char __attribute__((section(".custom_flash_block"))) custom_flash_buff[10];	//10 bytes

void CUSTOM_FUNC _led_toggle(uint32_t delay){
	RCC->AHBENR |= GPIOAEN;

    GPIOA->MODER &= ~(3U << (5 * 2)); // Clear mode bits for PA5
    GPIOA->MODER |=  (1U << (5 * 2)); // Set PA5 as output

	GPIOA->ODR ^= (1U<<5);

	delay_ms(delay);
}

int main(){

	while(1){
		_led_toggle(1000);
	}
}
