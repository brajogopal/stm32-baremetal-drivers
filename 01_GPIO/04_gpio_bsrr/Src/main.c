#include "stm32f030x8.h"


#define GPIOAEN       (1U<<17)
#define PIN5          (1U<<5)
#define LED_PIN       PIN5


int main (void){
	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~ (1U<<11);
	while(1){
		GPIOA->BSRR = LED_PIN;  // TO turn ON the LED
		for(int i=0 ;i<100000; i++){}

		GPIOA->BSRR = (1U<<21); //TO turn OFF the LED
		for(int i=0 ;i<100000; i++){}
	}
}
