#include "stm32f030x8.h"
#include "tim.h"

#define GPIOAEN       (1U<<17)
#define PIN5          (1U<<5)
#define LED_PIN       PIN5


int main (void){

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~ (1U<<11);

	tim1_1hz_init();

	while(1){

		while(!(TIM1->SR & TIM1_SR_UIF)){} //wait for UIF
		TIM1->SR &=~ TIM1_SR_UIF; //CLEAR UIF

		GPIOA->ODR ^= LED_PIN;  // TO turn ON the LED

}
}
