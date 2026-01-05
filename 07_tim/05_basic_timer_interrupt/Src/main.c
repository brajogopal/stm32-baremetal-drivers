#include "stm32f030x8.h"
#include "tim.h"

#define GPIOAEN       (1U<<17)
#define PIN5          (1U<<5)
#define LED_PIN       PIN5


int main (void){

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~ (1U<<11);

	tim3_1hz_interrupt_init();

	while(1){


}
}

void TIM3_IRQHandler(void){

	/*Clear Update Interrupt flag*/
	TIM3->SR &= ~TIM3_SR_UIF;

	//Do something
	GPIOA->ODR ^= LED_PIN;  // TO turn ON the LED

}
