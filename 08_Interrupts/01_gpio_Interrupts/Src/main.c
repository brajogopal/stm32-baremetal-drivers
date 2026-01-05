#include "exti.h"

#define GPIOAEN       (1U<<17)
#define PIN5          (1U<<5)
#define LED_PIN       PIN5

static void exti_callback(void);

int main (void){

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~ (1U<<11);

	pc13_exti_init();
	while(1){

	}

}
static void exti_callback(void){
	GPIOA->ODR ^= LED_PIN;
}


void EXTI4_15_IRQHandler(void){
	if((EXTI->PR & LINE13) != 0){
		EXTI->PR|=LINE13;//Clear PR flag
		exti_callback();//Do something
	}
}
