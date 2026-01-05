#include "stm32f030x8.h"
#include "systick.h"

#define GPIOAEN       (1U<<17)
#define PIN5          (1U<<5)
#define LED_PIN       PIN5


int main (void){

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~ (1U<<11);

	systick_1hz_interrupt();

	while(1){

}
}

void SysTick_Handler(void){
	//DO something
	GPIOA->ODR ^= LED_PIN;
}
